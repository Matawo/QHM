# Define the name algebra and the operations on it


class SimpleName:  # Class of simple name, i.e atomic name with a path in the binary tree (ex : u.rlr)
    def __init__(self, atomic, tree):
        self.atomic = atomic
        self.tree = tree

    def __hash__(self):
        return hash((self.atomic, self.tree))

    def __str__(self):
        tree_string = ""
        for i in self.tree:
            if i:
                tree_string += "l"
            else:
                tree_string += "r"
        return str(self.atomic) + '.' + tree_string

    def __repr__(self):
        return str(self)

    def __eq__(self, other):
        if type(other) != SimpleName:
            return False
        return (self.atomic == other.atomic) and (self.tree == other.tree)

    def __ne__(self, other):
        return not self == other

    def __gt__(self, other):
        if type(other) == ComposedName:
            return True
        if self.atomic > other.atomic:
            return True
        if self.atomic < other.atomic:
            return False
        return self.tree > other.tree

    def brother(self, brother):  # Test if the to names are brothers (ex u.rll and u.rlr)
        return (self.atomic == brother.atomic) and \
               (self.tree[:len(self.tree) - 1] == brother.tree[:len(brother.tree) - 1]) and \
               (self.tree != ()) and \
               (self.tree[-1])

    def left(self):  # left son
        return SimpleName(self.atomic, self.tree + (True,))

    def right(self):  # right son
        return SimpleName(self.atomic, self.tree + (False,))

    def split(self):
        return self.left(), self.right()

    def normalize(self):
        return self

    def smaller(self, other):
        if self < other:
            return self
        return other

    def smallest_simple(self):
        return self


class ComposedName:  # Class of composed names, i.e non-commutative conjonction of names ex : (u.l ^ v.rr)
    def __init__(self, name1, name2):
        self.name1 = name1
        self.name2 = name2

    def __str__(self):
        return '(' + str(self.name1) + '^' + str(self.name2) + ')'

    def __hash__(self):
        return hash((self.name1, self.name2))

    def __repr__(self):
        return '(' + str(self.name1) + '^' + str(self.name2) + ')'

    def __eq__(self, other):
        if type(other) != ComposedName:
            return False
        return (self.name1 == other.name1) and (self.name2 == other.name2)

    def __gt__(self, other):
        if type(other) == SimpleName:
            return False
        elif self.name1 > other.name1:
            return True
        elif self.name1 < other.name1:
            return False
        else:
            return self.name2 > other.name2

    def normalize(self):  # return a normalize version of the composed name (ex : (u.rll ^ u.rlr) -> u.rl)
        self.name1 = self.name1.normalize()
        self.name2 = self.name2.normalize()
        if isinstance(self.name1, SimpleName) and isinstance(self.name2, SimpleName):
            if self.name1.brother(self.name2):
                return SimpleName(self.name1.atomic, self.name1.tree[:len(self.name1.tree) - 1])
        return self

    def left(self):
        return self.name1

    def right(self):
        return self.name2

    def smallest_simple(self):
        s1 = self.name1.smallest_simple()
        s2 = self.name2.smallest_simple()
        return s1.smaller(s2)

    def smaller(self, other):
        return self.smallest_simple().smaller(other.smallest_simple())
