from pathlib import Path
from typing import List

from bs4 import BeautifulSoup as bs

from qemu_device_builder.qemu_device_model.register_map_model import SFR, BitField, BLOCK


class Parser:
    def __init__(self, register_map: Path):
        self.soup = None
        with open(register_map, "r", encoding='utf-8') as file:
            content = file.readlines()
            content = "".join(content)
            self.soup = bs(content, "xml")

    def get_block_with_name(self, name: str) -> List[BLOCK]:
        block_with_name = []
        if self.soup is not None:
            block_name = self.soup.find('BNAM', string=name)
            if block_name is not None:

                block = block_name.parent
                print('Module Found\n')
                block_with_name = self._get_block_attributes(block)
        return block_with_name

    def get_all_blocks(self) -> List[BLOCK]:
        block_list = []
        if self.soup is not None:
            block_list_nodes = self.soup.findAll('BLOCK')
            for block_node in block_list_nodes:
                block_list.extend(self._get_block_attributes(block_node))
        return block_list

    def _get_block_attributes(self, block: bs) -> List[BLOCK]:
        block_value = []
        name = block.find('BNAM').text
        print(f'Processing Block : {name}')
        instances = block.findAll('INSTANCE')
        for instance in instances:
            start = instance.find('ISTART').text
            end = instance.find('IEND').text
            inst_name = instance.find('INAME').text
            block_value.append(BLOCK(Name=name, InstanceName=inst_name, Start=start, End=end,
                                     SfrList=self.get_all_sfr(block, start, end), Size=""))
        return block_value

    def get_block(self) -> List[BLOCK]:
        block_instances = []
        if self.soup is not None:
            block_node = self.soup.find('BLOCK')
            block_instances = self._get_block_attributes(block_node)
        return block_instances

    def get_all_sfr(self, block: bs, start: str, end: str) -> List[SFR]:
        sfr_list = []
        if block is not None:
            sfrs = block.findAll('SFR')
            for sfr in sfrs:
                s = (self.parse_sfr(sfr))
                if self._is_sfr_part_of_instance(start, end, s.Addr):
                    sfr_list.append(s)
        return sfr_list

    @staticmethod
    def _is_sfr_part_of_instance(start: str, end: str, addr: str):
        start_addr = int(start, 16)
        end_addr = int(end, 16)
        req_addr = int(addr, 16)
        if start_addr <= req_addr < end_addr:
            return True
        else:
            return False

    def parse_sfr(self, sfr: bs) -> SFR:
        writable = False
        SNAM_Node = sfr.find('SNAM')
        LNAM_Node = sfr.find('LNAM')
        ADDR_Node = sfr.find('ADDR')
        RVAL_Node = sfr.find('RVAL')
        SNAM = "SFR_NAME_NOT_FOUND"
        LNAM = ""
        ADDR = "00000000"
        RVAL = "00000000"
        if SNAM_Node is not None:
            SNAM = SNAM_Node.text
        if LNAM_Node is not None:
            LNAM = LNAM_Node.text
        if ADDR_Node is not None:
            ADDR = ADDR_Node.text
        if RVAL_Node is not None:
            RVAL = RVAL_Node.text
        bitfields = sfr.findAll('BITS')
        bf_list = []
        for bf in bitfields:
            BITSNAME = bf.find('BITSNAME').text
            LOCH = bf.find('LOCH').text
            LOCL = bf.find('LOCL').text
            TYPE = bf.find('TYPE').text
            width = str((int(LOCH, 10) - int(LOCL, 10)) + 1)
            bf_list.append(BitField(Name=BITSNAME, Start=LOCL, End=LOCH, Access=TYPE, Width=width))
        writable_fields = [x for x in bf_list if 'w' in x.Access]
        if len(writable_fields) > 0:
            writable = True
        updated_bf_list = self._sort_bit_fields(bf_list)
        return SFR(Name=SNAM, Desc=LNAM, Addr=ADDR, RVal=RVAL, BitFields=updated_bf_list,Writable=writable)

    @staticmethod
    def _sort_bit_fields(bit_fields: List[BitField]):
        print(bit_fields)
        if len(bit_fields) > 0:
            actual_bit_fields = sorted(bit_fields, key=lambda x: int(x.Start, 10))
            print('......................................')
            print(f'after sorting {actual_bit_fields}')
            modified_bf = actual_bit_fields
            missing_bf = []
            i = 0

            while i < len(actual_bit_fields) - 1:
                if i == 0 and actual_bit_fields[i].Start != '0':

                    missing_bf_elm = BitField(Name=f'ReservedPre{i}', Start=str(0), End=str(int(actual_bit_fields[i].Start,
                                                                                             10) - 1),
                                              Width=str(actual_bit_fields[i].Start), Access='r')
                    missing_bf.append(missing_bf_elm)
                    print(f' prefix {missing_bf_elm}')

                next_start = int(actual_bit_fields[i].Start, 10) + int(actual_bit_fields[i].Width, 10)
                actual_next = int(actual_bit_fields[i + 1].Start, 10)
                next_width = actual_next - next_start
                if next_width != 0:

                    missing_bf_elm = BitField(Name=f'Reserved{i}', Start=str(next_start), End=str(actual_next - 1),
                                              Width=str(next_width), Access='r')
                    missing_bf.append(missing_bf_elm)
                    print(f' Mid fix {missing_bf_elm}')
                    print(f'Name={actual_bit_fields[i].Name} Start={actual_bit_fields[i].Start} Next Start = {actual_bit_fields[i+1].Start}')
                i += 1

            if i == len(actual_bit_fields) - 1:
                last_start = int(actual_bit_fields[i].Start, 10) + int(actual_bit_fields[i].Width, 10)
                last_width = 32 - last_start
                if last_width != 0:
                    print(f'Name={actual_bit_fields[i].Name} Start={actual_bit_fields[i].Start} Width = {actual_bit_fields[i].Width}')
                    print(int(actual_bit_fields[i].Width, 10))
                    missing_bf_elm = BitField(Name=f'Reserved{i}', Start=str(last_start), End='31',
                                          Width=str(last_width), Access='r')
                    missing_bf.append(missing_bf_elm)
                    print(f' End fix {missing_bf_elm}')

            modified_bf.extend(missing_bf)
            modified_bf = sorted(modified_bf, key=lambda x: int(x.Start,10))
            print(f'after reserve {actual_bit_fields}')
            print('......................................')
            return modified_bf
        return bit_fields
