from scipy import linalg
from QHM import *
import numpy as np
from math import *
import time


def run(qhm, nb_sc, nb_step):  # Lance un run sur qhm, de nb_sc secondes, de nb_step étapes
    time_rotation = 0
    time_interaction = 0
    time_shift = 0
    time_creation = 0
    total_time = 0
    start = time.time()
    print("\n____________________________")
    print(qhm)
    print("Rotation :\n", qhm.rotationOp)
    print("Creation :\n", qhm.creation_op)
    print("Interaction :\n", qhm.interactionOp)
    data = []
    n = 0
    while time.time() - start< nb_sc and n < nb_step:
        print(n, qhm.number_of_graphs(), time.time() - start)
        # Etape de rotation Ur |--L--> = a |--L--> + b |--R-->
        time_rotation -= time.time()
        qhm.update_dict(qhm.rotation_step())
        time_rotation += time.time()
        # Etape de creation Ur |--O--> = a |--LR--> + b |--O-->
        time_creation -= time.time()
        qhm.update_dict(qhm.creation_destruction_step())
        time_creation += time.time()
        # Etape de shift, exemple :|-L--O-> = |--O--L--> (pour l'instant il n'y a que l'identité)
        time_shift -= time.time()
        qhm.update_dict(qhm.shifting_step())
        time_shift += time.time()
        # Etape d'interaction, exemple : Ui|-LR-> = a|-LR-> + b|-L--R->
        time_interaction -= time.time()
        qhm.update_dict(qhm.interaction_step())
        time_interaction += time.time()
        data += [qhm.size_probability()]
        total_time = time_rotation + time_interaction + time_shift
        n += 1
    total_time = time.time() - start
    print("rotation :", time_rotation, "sc ", time_rotation / total_time * 100, "%")
    print("creation/destruction :", time_creation, "sc ", time_rotation / total_time * 100, "%")
    print("interaction :", time_interaction, "sc ", time_interaction / total_time * 100, "%")
    print("shift :", time_shift, "sc ", time_shift / total_time * 100, "%")
    print("Nombres d'étapes, nombres de graphes", n, qhm.number_of_graphs())
    print(qhm.norm())
    print("____________________________\n")
    return data


# Definition des matrices unitaires:
identity = np.mat([[1., 0.], [0., 1.]])
hadamard = np.mat([[1/sqrt(2), 1/sqrt(2)], [1/sqrt(2), -1/sqrt(2)]])
theta = pi/4
masse = 0.1
pauli_x = np.mat([[0., 1.], [1., 0.]])
unitary1 = linalg.expm(pauli_x * complex(0, masse))
unitary2 = linalg.expm(pauli_x * complex(0, pi/2 + masse))

# Quelques graphes intéressant
# Définition du graphe -1:LR--2:L- (L note une particule qui va vers la gauche, R une particule vers la droite)
gmn1 = GraphModuloName(2, ((True, True), (True, False)))  # Classique pseudo-stable à 20 étapes
gmn2 = GraphModuloName(2, ((True, True), (True, True)))  # Classique oscillation
gmn3 = GraphModuloName(4, ((True, True), (True, True), (True, False), (False, True)))
# Pseudo oscillation observable à 200 étapes

# Paires d'opérateurs à intéressant à étudier
ops1 = (identity, hadamard, identity)
ops2 = (identity, unitary1, identity)
ops3 = (identity, unitary2, identity)
ops4 = (unitary1, pauli_x, identity)
ops5 = (unitary1, hadamard, identity)
ops6 = (identity, pauli_x, unitary1)