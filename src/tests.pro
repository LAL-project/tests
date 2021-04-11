TEMPLATE = subdirs

SUBDIRS += \
    common \
	generate \
    graphs \
	internal \
	io \
	linarr \
	memory \
	numeric \
	properties \
	utilities \
	tests

generate.depends = common
graphs.depends = common
internal.depends = common
io.depends = common
linarr.depends = common
memory.depends = common
numeric.depends = common
properties.depends = common
utilities.depends = common
tests.depends = common
