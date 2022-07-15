TEMPLATE = subdirs

SUBDIRS += \
	detail \
	common \
	generate \
	graphs \
	io \
	linarr \
	memory \
	numeric \
	properties \
	utilities \
	tests

detail.depends = common
generate.depends = common
graphs.depends = common
io.depends = common
linarr.depends = common
memory.depends = common
numeric.depends = common
properties.depends = common
utilities.depends = common
tests.depends = common
