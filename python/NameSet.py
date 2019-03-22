from NameAlgebra import *


class NameSet:
    error_margin = 0.1 ** 9

    def __init__(self, hashmap, symmetry=None):
        self.map = hashmap
        if symmetry is None:
            symmetry = [0]
        self.symmetry = symmetry

    def __str__(self):
        return str(self.map)

    def __repr__(self):
        return "NamedGraph:" + str(self)

    def __copy__(self):
        new_name_set = NameSet(dict(), self.symmetry)
        new_name_set.add_all(self)
        return new_name_set

    @ staticmethod
    def range(n, amp=1.0):
        naming = tuple(SimpleName(i, ()) for i in range(n))
        nameset = NameSet({naming: amp})
        return nameset

    def add(self, name, amp):
        n = len(name)
        name = tuple(max([[name[(i+s) % n] for i in range(n)] for s in self.symmetry]))
        old_amp = self.map.pop(name, 0)
        if abs(amp + old_amp) > NameSet.error_margin:
            self.map[name] = amp + old_amp

    def add_all(self, other):
        for name, amp in other.map.items():
            self.add(name, amp)
        self.clean_up()

    def smallest_symmetry(self):
        new_name_set = NameSet(dict(), self.symmetry)
        new_name_set.add_all(self)
        return new_name_set

    def multiply_amps(self, value):
        new_name_set = NameSet(dict(), self.symmetry)
        for names, amp in self.map.items():
            new_name_set.add(names, amp * value)
        return new_name_set

    @staticmethod
    def modify_tuple(name, naming_infos):
        new_tuple = []
        for naming_info in naming_infos:
            if type(naming_info) == ComposedName:
                a, b = naming_info.left().atomic, naming_info.right().atomic
                to_add = ComposedName(name[a], name[b]).normalize()
                new_tuple.append(to_add)
                # print(naming_infos)
            elif naming_info.tree == ():
                new_tuple.append(name[naming_info.atomic])
            elif naming_info.tree[0]:
                new_tuple.append(name[naming_info.atomic].left())
            else:
                new_tuple.append(name[naming_info.atomic].right())
        return tuple(new_tuple)

    def clean_up(self):
        to_remove = set()
        for name, value in self.map.items():
            if abs(value) < NameSet.error_margin:
                to_remove.add(name)
        for name in to_remove:
            self.map.pop(name)

    def get_modify(self, naming_infos):
        new_name_set = NameSet(dict())
        for names, amp in self.map.items():
            new_name_set.add(NameSet.modify_tuple(names, naming_infos), amp)
        return new_name_set

    def offset(self, offset):
        new_name_set = NameSet(dict(), self.symmetry)
        for names, amp in self.map.items():
            new_name_set.add(tuple(names[(i+offset) % len(names)] for i in range(len(names))), amp)
        return new_name_set
