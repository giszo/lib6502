env = Environment()

lib = env.SharedLibrary("6502", source = ["src/cpu.cpp", "src/ora.cpp"], CPPFLAGS = ["-O2", "-Wall", "-std=c++11"], CPPPATH = ["include/"])
env["lib"] = lib

SConscript(dirs = ["test"], exports = ["env"])

env.Alias("install", env.Install("/usr/lib", lib))
env.Alias("install", env.Install("/usr/include/lib6502", "include/lib6502/cpu.h"))
env.Alias("install", env.Install("/usr/include/lib6502", "include/lib6502/memory.h"))
env.Alias("install", env.Install("/usr/include/lib6502", "include/lib6502/makestring.h"))

Default(lib)
