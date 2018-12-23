import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from utils import *

# Utilisation
gmn3 = GraphModuloName(10, ((False, False),)*10)

# Définition du QHM :
qhm = QHM({gmn3: NameSet.range(gmn3.size)}, identity, pauli_x, unitary1)

# Marge d'erreur à faire varier, 10**-3 est relativement imprécis, 10**-6 suffit pour la grande majorité des cas.
QHM.error_margin = 10.0**(-4)
NameSet.error_margin = 10.0**(-4)

# Tests, on souhaiterait calculer environ 200 étapes pour la majorité des combinaisons opérateurs/graphes dans utils
data = run(qhm, 15, 15)

# Mise en forme des données
max_size = max([len(x) for x in data])
X, Y = np.meshgrid(np.arange(max_size), np.arange(len(data)))
Z = np.array([[abs(z) for z in step_amp] + [0] * (max_size - len(step_amp)) for step_amp in data])

# Affichage graphique
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot_wireframe(Y, X, Z)
ax.set_xlabel('Temps')
ax.set_ylabel('Taille')
ax.set_zlabel('Proba')

for spine in ax.spines.values():
    spine.set_visible(False)

plt.show()

