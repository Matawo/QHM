# Version 2, calcul modulo renommage
from GraphModuloName import *
from NameSet import *
import copy


def lexico_order(sequence):
    greatest = list(sequence)
    index = 0
    for i in range(len(sequence)):
        current = [sequence[(i + j) % len(sequence)] for j in range(len(sequence))]
        if greatest < current:
            greatest = current
            index = i
    # print(sequence, index, greatest)
    return index, tuple(greatest)


def count_true(sequence):
    counter = 0
    for (a, b) in sequence:
        if a:
            counter += 1
        if b:
            counter += 1
    return counter


class QHM:

    error_margin = 10**(-9)

    def __init__(self, superposition, rotation_op, interaction_op, creation_op):
        self.superposition = superposition
        self.interactionOp = interaction_op
        self.rotationOp = rotation_op
        self.creation_op = creation_op

    def __str__(self):
        string = ""
        for graph, name_set in self.superposition.items():
            string += "=" + str(graph) + " " + str(name_set) + "="
        return string

    def update_dict(self, superposition):  # Met à jour l'état du QHM
        self.superposition = superposition

    def size_probability(self):  # Calcul de la probabilité d'observer chacune des tailles du graphe
        max_size = max([gmn.size for gmn in self.superposition.keys()])
        size_list = [0] * (max_size + 1)
        for gmn, name_set in self.superposition.items():
            size_list[gmn.size] += sum([abs(amp) ** 2 for amp in name_set.map.values()])
        return size_list

    def norm(self):  # Calcul de la norme
        total = 0
        for name_set in self.superposition.values():
            for amp in name_set.map.values():
                total += abs(amp) ** 2
        return total

    def shifting_step(self):
        new_dict = dict()
        for gmn, name_set in self.superposition.items():
            offset, new_gmn_array = lexico_order(gmn.shifting())
            name_set = name_set.offset(offset)
            name_set.symmetry = gmn.symmetry
            name_set = name_set.smallest_symmetry()
            new_dict[GraphModuloName(gmn.size, new_gmn_array)] = name_set
        return new_dict

    def rotation_step(self):
        new_dict = dict()
        for gmn, name_set in self.superposition.items():  # Pour chaque graphe de la superposition
            for new_gmn_array, value in gmn.rotation(self.rotationOp).items():
                offset, new_gmn_array = lexico_order(new_gmn_array)
                new_gmn = GraphModuloName(gmn.size, new_gmn_array)
                new_name_set = name_set.multiply_amps(value)
                new_name_set.symmetry = new_gmn.symmetry
                new_name_set = new_name_set.offset(offset)
                old_name_set = new_dict.get(new_gmn, NameSet(dict()))
                new_name_set.add_all(old_name_set)
                new_dict[new_gmn] = new_name_set
        QHM.clean_up(new_dict)
        return new_dict

    def creation_destruction_step(self):
        new_dict = dict()
        for gmn, name_set in self.superposition.items():  # Pour chaque graphe de la superposition
            for new_gmn_array, value in gmn.creation_destruction(self.creation_op).items():
                offset, new_gmn_array = lexico_order(new_gmn_array)
                new_gmn = GraphModuloName(gmn.size, new_gmn_array)
                new_name_set = name_set.multiply_amps(value)
                new_name_set.symmetry = new_gmn.symmetry
                new_name_set = new_name_set.offset(offset)
                old_name_set = new_dict.get(new_gmn, NameSet(dict()))
                new_name_set.add_all(old_name_set)
                new_dict[new_gmn] = new_name_set
        QHM.clean_up(new_dict)
        return new_dict

    def interaction_step(self):
        new_dict = dict()
        for gmn, name_set in self.superposition.items():  # Pour chaque graphe de la superposition
            # print("_________ \n", gmn, name_set)
            for new_gmn_array, (value, naming) in gmn.interaction(self.interactionOp).items():
                # if count_true(new_gmn_array) != 4 :
                #     print("WARNING", new_gmn_array)
                offset, new_gmn_array = lexico_order(new_gmn_array)
                new_gmn = GraphModuloName(len(new_gmn_array), new_gmn_array)
                new_name_set = name_set.get_modify(naming)
                new_name_set = new_name_set.multiply_amps(value)
                new_name_set.symmetry = new_gmn.symmetry
                new_name_set = new_name_set.offset(offset)
                old_name_set = new_dict.get(new_gmn, NameSet(dict()))
                new_name_set.add_all(old_name_set)
                new_dict[new_gmn] = new_name_set
        QHM.clean_up(new_dict)
        return new_dict

    @staticmethod
    def clean_up(superposition):
        to_remove = set()
        for gmn, name_set in superposition.items():
            if len(name_set.map) == 0:
                to_remove.add(gmn)
        for gmn in to_remove:
            superposition.pop(gmn)

    def number_of_graphs(self):
        return sum([len(name_set.map) for name_set in self.superposition.values()])

    def minus(self, other):
        result = QHM(copy.deepcopy(self.superposition), self.rotationOp, self.interactionOp, self.creation_op)
        for gmn, name_set in other.superposition.items():
            other_name_set = name_set.multiply_amps(-1)
            result_name_set = result.superposition.get(gmn, NameSet(dict()))
            result_name_set.add_all(other_name_set)
        QHM.clean_up(result.superposition)
        return result