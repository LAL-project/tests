import laldebug as lal

"""
class asdf:
	def __init__(self):
		self.qwer = 0
	def __repr__(self):
		return str(self.qwer)

def __add__(self, k):
	return self.qwer + k
def __radd__(self, k):
	return self.qwer + k
setattr(asdf, __add__.__name__, __add__)
setattr(asdf, __radd__.__name__, __radd__)

k = asdf()
print(k + 4)
print(4 + k)
"""

print("=======")
print("INTEGER")

i = lal.numeric.integer(3)
print("i=", i)
print("01. i + 3=", i + 3)
print("02. 4 + i=", 4 + i)

print("03. i - 3=", i - 3)
print("04. 4 - i=", 4 - i)

print("05. i*3=", i*3)
print("06. i*-3=", i*-3)
print("07. 4*i=", 4*i)
print("08. -4*i=", -4*i)

print("09. i/3=", i/3)
print("10. i/-3=", i/-3)
print("11. 4/i=", 4/i)
print("12. -4/i=", -4/i)

print("13. i^2=", i**2)
print("14. i^3=", i**3)
i **= 3
print("i=", i)
print("15. i^=3", i)

i = lal.numeric.integer(2)
print("i=", i)
print("16. 3^i=", 3**i)
print("17. (-3)^i=", (-3)**i)
i = lal.numeric.integer(-2)
print("18. i=", i)
print("19. 3^i=", 3**i)
print("20. (-3)^i=", (-3)**i)

i = lal.numeric.integer(-3)
print("i=", i)
print("21. 3^i=", 3**i)
print("22. (-3)^i=", (-3)**i)

print("---------------------")

i = lal.numeric.integer(-3)
print("i=", i)
print("23. i + 3=", i + 3)
print("24. 4 + i=", 4 + i)

print("25. i - 3=", i - 3)
print("26. 4 - i=", 4 - i)

print("27. i*3=", i*3)
print("28. i*-3=", i*-3)
print("29. 4*i=", 4*i)
print("30. -4*i=", -4*i)

print("31. i/3=", i/3)
print("32. i/-3=", i/-3)
print("33. 4/i=", 4/i)
print("34. -4/i=", -4/i)


print("========")
print("RATIONAL")

i = lal.numeric.rational(3)
print("i=", i)
print("35. i + 3=", i + 3)
print("36. 4 + i=", 4 + i)

print("37. i - 3=", i - 3)
print("38. 4 - i=", 4 - i)
print("39. i*3=", i*3)
print("40. i*-3=", i*-3)
print("41. 4*i=", 4*i)
print("42. -4*i=", -4*i)

print("43. i/3=", i/3)
print("44. i/-3=", i/-3)
print("45. 4/i=", 4/i)
print("46. -4/i=", -4/i)

print("47. i^2=", i**2)
print("48. i^3=", i**3)
i **= 3
print("i=", i)
print("49. i^=3", i)

i = lal.numeric.rational(2)
print("i=", i)
print("50. 3^i=", 3**i)
print("51. (-3)^i=", (-3)**i)
i = lal.numeric.rational(-2)
print("i=", i)
print("52. 3^i=", 3**i)
print("53. (-3)^i=", (-3)**i)

i = lal.numeric.rational(-3)
print("i=", i)
print("54. 3^i=", 3**i)
print("55. (-3)^i=", (-3)**i)

print("---------------------")

i = lal.numeric.rational(-3)
print("i=", i)
print("56. i + 3=", i + 3)
print("57. 4 + i=", 4 + i)

print("58. i - 3=", i - 3)
print("59. 4 - i=", 4 - i)

print("60. i*3=", i*3)
print("61. i*-3=", i*-3)
print("62. 4*i=", 4*i)
print("63. -4*i=", -4*i)

print("64. i/3=", i/3)
print("65. i/-3=", i/-3)
print("66. 4/i=", 4/i)
print("67. -4/i=", -4/i)


print("68. 5^(2/5)=", 5**lal.numeric.rational(2,5))

print("---------------------")
i = lal.numeric.integer(3)
print("i=", i)
i *= 3
print("69. i*=3 ->", i)
i += 3
print("70. i+=3 ->", i)
i -= 3
print("71. i-=3 ->", i)
i /= 3
print("72. i/=3 ->", i)

i = lal.numeric.rational(10,3)
print("i=", i)
i *= 3
print("69. i*=3 ->", i)
i += 3
print("70. i+=3 ->", i)
i -= 3
print("71. i-=3 ->", i)
i /= 3
print("72. i/=3 ->", i)
