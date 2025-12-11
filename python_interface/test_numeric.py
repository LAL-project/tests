import laloptimized as lal

# #################################################################### #
# ############################ INTEGER ############################### #
# #################################################################### #

i = lal.numeric.integer(3)
assert str(i) == "3"
assert i + 3 == lal.numeric.integer(6)
assert i + 3 == 6
assert 4 + i == 7
assert 4 + i == lal.numeric.integer(7)

# 03. i - 3 = 0
assert i - 3 == 0
assert i - 3 == lal.numeric.integer(0)

# 04. 4 - i = 1
assert 4 - i == 1
assert 4 - i == lal.numeric.integer(1)

# 05. i*3 = 9
assert i * 3 == 9
assert i * 3 == lal.numeric.integer(9)

# 06. i*-3 = -9
assert i * -3 == -9
assert i * -3 == lal.numeric.integer(-9)

# 07. 4*i = 12
assert 4 * i == 12
assert 4 * i == lal.numeric.integer(12)

# 08. -4*i = -12
assert -4 * i == -12
assert -4 * i == lal.numeric.integer(-12)

# 09. i/3 = 1
assert i / 3 == 1
assert i / 3 == lal.numeric.integer(1)

# 10. i/-3 = -1
assert i / -3 == -1
assert i / -3 == lal.numeric.integer(-1)

# 11. 4/i = 1
assert 4 / i == lal.numeric.rational(4, 3)

# 12. -(4/i) = -1
assert -(4 / i) == -lal.numeric.rational(4, 3)
assert (-4) / i == -lal.numeric.rational(4, 3)
assert 4 / (-i) == -lal.numeric.rational(4, 3)

# 13. i^2 = 9
assert i ** 2 == 9
assert i ** 2 == lal.numeric.integer(9)

# 14. i^3 = 27
assert i ** 3 == 27
assert i ** 3 == lal.numeric.integer(27)

# i **= 3 -> i becomes 27
i **= 3
assert i == 27
assert i == lal.numeric.integer(27)

# 15. i^=3 (already asserted above)
assert i == 27

# i = 2
i = lal.numeric.integer(2)
assert i == 2
assert i == lal.numeric.integer(2)

# 16. 3^i = 9
assert 3 ** i == 9
assert 3 ** i == lal.numeric.integer(9)

# 17. (-3)^i = 9
assert (-3) ** i == 9
assert (-3) ** i == lal.numeric.integer(9)

i = lal.numeric.integer(-2)

# 18. i = -2
assert i == -2
assert -2 == i
assert i == lal.numeric.integer(-2)
assert lal.numeric.integer(-2) == i

# 19. 3^i = 3**-2 = 1/9
assert 3 ** i == lal.numeric.rational(1, 9)

# 20. (-3)^i = (-3)**-2 = 1/9
assert (-3) ** i == lal.numeric.rational(1, 9)

# i = -3
i = lal.numeric.integer(-3)
assert i == lal.numeric.integer(-3)

# 21. 3^i = 3**-3 = 1/27
assert 3 ** i == lal.numeric.rational(1, 27)

# 22. (-3)^i = (-3)**-3 = -1/27
assert (-3) ** i == -lal.numeric.rational(1, 27)
assert (-3) ** i == lal.numeric.rational(-1, 27)

# ---------------------

i = lal.numeric.integer(-3)
assert i == lal.numeric.integer(-3)

# 23. i + 3 = 0
assert i + 3 == 0
assert i + 3 == lal.numeric.integer(0)

# 24. 4 + i = 1
assert 4 + i == 1
assert 4 + i == lal.numeric.integer(1)

# 25. i - 3 = -6
assert i - 3 == -6
assert i - 3 == lal.numeric.integer(-6)

# 26. 4 - i = 7
assert 4 - i == 7
assert 4 - i == lal.numeric.integer(7)

# 27. i*3 = -9
assert i * 3 == -9
assert i * 3 == lal.numeric.integer(-9)

# 28. i*-3 = 9
assert i * -3 == 9
assert i * -3 == lal.numeric.integer(9)

# 29. 4*i = -12
assert 4 * i == -12
assert 4 * i == lal.numeric.integer(-12)

# 30. -4*i = 12
assert -4 * i == 12
assert -4 * i == lal.numeric.integer(12)

# 31. i/3 = -1
assert i / 3 == -1
assert i / 3 == lal.numeric.integer(-1)

# 32. i/-3 = 1
assert i / -3 == 1
assert i / -3 == lal.numeric.integer(1)

# #################################################################### #
# ########################### RATIONAL ############################### #
# #################################################################### #

i = lal.numeric.rational(3)
assert i == lal.numeric.rational(3)

# 35. i + 3 = 6
assert i + 3 == 6
assert i + 3 == lal.numeric.rational(6)

# 36. 4 + i = 7
assert 4 + i == 7
assert 4 + i == lal.numeric.rational(7)

# 37. i - 3 = 0
assert i - 3 == 0
assert i - 3 == lal.numeric.rational(0)

# 38. 4 - i = 1
assert 4 - i == 1
assert 4 - i == lal.numeric.rational(1)

# 39. i*3 = 9
assert i * 3 == 9
assert i * 3 == lal.numeric.rational(9)

# 40. i*-3 = -9
assert i * -3 == -9
assert i * -3 == lal.numeric.rational(-9)

# 41. 4*i = 12
assert 4 * i == 12
assert 4 * i == lal.numeric.rational(12)

# 42. -4*i = -12
assert -4 * i == -12
assert -4 * i == lal.numeric.rational(-12)

# 43. i/3 = 1
assert i / 3 == 1
assert i / 3 == lal.numeric.rational(1)

# 44. i/-3 = -1
assert i / -3 == -1
assert i / -3 == lal.numeric.rational(-1)

# 45. 4/i = 4/3
assert 4 / i == lal.numeric.rational(4, 3)

# 46. -4/i = -4/3
assert -4 / i == lal.numeric.rational(-4, 3)

# 47. i^2 = 9
assert i ** 2 == 9
assert i ** 2 == lal.numeric.rational(9)

# 48. i^3 = 27
assert i ** 3 == 27
assert i ** 3 == lal.numeric.rational(27)

# i **= 3 -> i becomes 27 (rational)
i **= 3
assert i == 27
assert i == lal.numeric.rational(27)

# 49. i^=3 (already asserted)
assert i == lal.numeric.rational(27)

# i = 2 (rational)
i = lal.numeric.rational(2)
assert i == lal.numeric.rational(2)

# 50. 3^i = 3**2 = 9
assert 3 ** i == 9

# 51. (-3)^i = 9
assert (-3) ** i == 9

# i = -2 (rational)
i = lal.numeric.rational(-2)
assert i == lal.numeric.rational(-2)

# 52. 3^i = 3**-2 = 1/9
assert 3 ** i == 1/9

# 53. (-3)^i = (-3)**-2 = 1/9
assert (-3) ** i == 1/9

# i = -3 (rational)
i = lal.numeric.rational(-3)
assert i == lal.numeric.rational(-3)

# 54. 3^i = 3**-3 = 1/27
assert 3 ** i == 1/27

# 55. (-3)^i = (-3)**-3 = -1/27
assert (-3) ** i == -1/27

# ---------------------

i = lal.numeric.rational(-3)
assert i == lal.numeric.rational(-3)

# 56. i + 3 = 0
assert i + 3 == lal.numeric.rational(0)
assert i + 3 == 0

# 57. 4 + i = 1
assert 4 + i == lal.numeric.rational(1)
assert 4 + i == 1

# 58. i - 3 = -6
assert i - 3 == lal.numeric.rational(-6)
assert i - 3 == -6

# 59. 4 - i = 7
assert 4 - i == lal.numeric.rational(7)
assert 4 - i == 7

# 60. i*3 = -9
assert i * 3 == lal.numeric.rational(-9)
assert i * 3 == -9

# 61. i*-3 = 9
assert i * -3 == lal.numeric.rational(9)
assert i * -3 == 9

# 62. 4*i = -12
assert 4 * i == lal.numeric.rational(-12)
assert 4 * i == -12

# 63. -4*i = 12
assert -4 * i == lal.numeric.rational(12)
assert -4 * i == 12

# 64. i/3 = -1
assert i / 3 == lal.numeric.rational(-1)
assert i / 3 == -1

# 65. i/-3 = 1
assert i / -3 == lal.numeric.rational(1)
assert i / -3 == 1

# 66. 4/i = 4/(-3) = -4/3
assert 4 / i == lal.numeric.rational(-4, 3)

# 67. -4/i = -4/(-3) = 4/3
assert -4 / i == lal.numeric.rational(4, 3)

# 68. 5^(2/5) -> check by raising result to 5 equals 25
val = 5 ** lal.numeric.rational(2, 5)
# (5^(2/5))**5 == 5^2 == 25
assert ((val ** 5) - 25) <= 1e-6

# ---------------------
i = lal.numeric.integer(3)
assert i == lal.numeric.integer(3)

# i *= 3 -> 9
i *= 3
assert i == lal.numeric.integer(9)
assert i == 9

# i += 3 -> 12
i += 3
assert i == lal.numeric.integer(12)
assert i == 12

# i -= 3 -> 9
i -= 3
assert i == lal.numeric.integer(9)
assert i == 9

# i /= 3 -> 3
i /= 3
# depending on implementation, i may become rational(3) or integer(3); check both:
assert (i == lal.numeric.integer(3)) or (i == lal.numeric.rational(3)) or (i == 3)

# rational sequence
i = lal.numeric.rational(10, 3)
assert i == lal.numeric.rational(10, 3)

# i *= 3 -> 10
i *= 3
assert i == lal.numeric.rational(10)
assert i == 10

# i += 3 -> 13
i += 3
assert i == lal.numeric.rational(13)
assert i == 13

# i -= 3 -> 10
i -= 3
assert i == lal.numeric.rational(10)
assert i == 10

# i /= 3 -> 10/3
i /= 3
assert i == lal.numeric.rational(10, 3)
