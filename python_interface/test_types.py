import lal

n1 = lal.types.node_t(3)
n2 = lal.types.node_t(4)

assert n1 < n2
assert n1 <= n2
assert not (n1 > n2)
assert not (n1 >= n2)
assert not (n1 + n2 < n2)
assert not (n1 + n2 <= n2)
assert n1 + n2 > n2
assert n1 + n2 >= n2

assert n1 < 4
assert n1 <= 4
assert not (n1 > 4)
assert not (n1 >= 4)

assert 4 > n1
assert 4 >= n1
assert not (4 < n1)
assert not (4 <= n1)

assert not (n1 + 3 < 4)
assert not (3 + n1 < 4)

assert n1 == 3
assert 3 == n1
assert n1 != 5
assert 5 != n1
n1 += 1
assert n1 == 4
assert n1 != 5
n1 -= 1
assert n1 == 3
assert n1 != 5

assert n1 != n2
n1 += 1
assert n1 == n2

assert n1 + 1 != n2
assert 1 + n1 != n2
assert n2 != n1 + 1
assert n2 != 1 + n1

print("All assertions passed!")
