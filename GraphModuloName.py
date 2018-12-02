# Graph modulo isomorphisme
from NameAlgebra import *


def count_true(sequence):
    counter = 0
    for (a, b) in sequence:
        if a:
            counter += 1
        if b:
            counter += 1
    return counter


class GraphModuloName:
    def __init__(self, size, particles):
        self.size = size
        self.graph = particles
        self.symmetry = self.symmetry_indexes()

    def __str__(self):
        string = "Graph:"
        for node in self.graph:
            if node == (False, False):
                string += "O"
            if node[0]:
                string += "L"
            if node[1]:
                string += "R"
            string += "--"
        return string

    def __repr__(self):
        string = "Graph:"
        for node in self.graph:
            if node == (False, False):
                string += "O"
            if node[0]:
                string += "L"
            if node[1]:
                string += "R"
            string += "--"
        return string

    def __eq__(self, other):
        return self.graph == other.graph

    def __hash__(self):
        return hash(self.graph)

    def symmetry_indexes(self):
        indexes = set(range(self.size))
        i = 0
        while len(indexes) > 1 and i < self.size:
            maximum = max([self.graph[(i+j) % self.size] for j in indexes])
            minus_set = set()
            for index in indexes:
                if self.graph[(index + i) % self.size] < maximum:
                    minus_set.add(index)
            indexes = indexes - minus_set
            i += 1
        return indexes

    def shifting(self):  # Return the graph obtained after a shifting phase
        particles = [(False, False)] * self.size
        for i in range(self.size):  # Shift the left
            particles[i] = (self.graph[(i + 1) % self.size][0], self.graph[i - 1][1])
        return particles

    def rotation(self, rotation_op):
        map_temp1 = {self.graph: 1.}
        for i in range(self.size):
            map_temp2 = {}
            for particles, value in map_temp1.items():
                # Calcul de l'évolution du graphe en position i
                new_particles = list(particles)
                new_particles[i] = (not particles[i][0], not particles[i][1])
                new_particles = tuple(new_particles)
                if particles[i] == (True, False):
                    map_temp2.update({particles: value * rotation_op.item(0, 0)})
                    map_temp2.update({new_particles: value * rotation_op.item(0, 1)})
                elif particles[i] == (False, True):
                    map_temp2.update({new_particles: value * rotation_op.item(1, 0)})
                    map_temp2.update({particles: value * rotation_op.item(1, 1)})
                else:
                    map_temp2.update({particles: value})
            map_temp1 = map_temp2
        return map_temp1

    def interaction(self, interaction_op):  # Interaction sur un vecteur de la base canonique #
        map_temp1 = {(): (1, ())}
        # print("_________________\n", graph_o)
        i = 0
        while i < self.size:
            # print("step ", i)
            map_temp2 = {}
            node = self.graph[i]
            node_name = SimpleName(i, ())
            next_node = self.graph[(i + 1) % self.size]
            for (g, (value, names_infos)) in map_temp1.items():
                # Calcul de l'évolution du graphe en position i
                successor_neutral = g[:]
                successor_neutral += (node,)
                successor_interacting = g[:]
                if node == (True, True):  # Cas classique split
                    #  Cas sans split
                    map_temp2.update({successor_neutral: (value * interaction_op.item(0, 0),
                                                          names_infos + (node_name,))})
                    #  Cas avec split
                    successor_interacting += ((True, False), (False, True))
                    map_temp2.update({successor_interacting: (value * interaction_op.item(0, 1),
                                                              names_infos + (node_name.left(), node_name.right()))})

                elif node == (True, False) and next_node == (False, True):  # Cas classique de merge
                    #  Cas sans merge
                    successor_neutral += (next_node,)
                    map_temp2.update({successor_neutral: (value * interaction_op.item(1, 1),
                                                          names_infos + (node_name, SimpleName(i+1, ())))})
                    #  Cas avec merge
                    successor_interacting += ((True, True),)
                    new_name_infos = names_infos + (ComposedName(node_name, SimpleName(i+1, ())),)
                    map_temp2.update({successor_interacting: (value * interaction_op.item(1, 0), new_name_infos)})
                    # print("2", map_temp2)
                elif (i != 0) or (self.graph[0] != (False, True)) or \
                        (self.graph[self.size - 1] != (True, False)):  # Cas classique sans interaction en 0
                    map_temp2.update({successor_neutral: (value, names_infos + (node_name,))})
                else:  # Graphe (False, True)-...-(True, False) # Interaction en 0
                    map_temp2.update({successor_interacting: (value, names_infos)})
            map_temp1 = map_temp2
            i += 1
            if node == (True, False) and next_node == (False, True):
                i += 1
        return map_temp1

    def nb_particles(self):
        n = 0
        for node in self.graph:
            if node[0]:
                n += 1
            if node[1]:
                n += 1
        return n
