env = Environment()

sources = [
    "cpu.cpp",
    "ora.cpp",
    "lda.cpp",
    "sta.cpp",
    "sbc.cpp",
    "cmp.cpp",
    "asl.cpp",
    "and.cpp",
    "adc.cpp"
]

env["lib"] = env.SharedLibrary(
    "6502",
    source = ["src/%s" % x for x in sources],
    CPPFLAGS = ["-O2", "-Wall", "-std=c++11"],
    CPPPATH = ["include/"]
)

SConscript(dirs = ["test"], exports = ["env"])

env.Alias("install", env.Install("/usr/lib", env["lib"]))
env.Alias("install", env.Install("/usr/include/lib6502", "include/lib6502/cpu.h"))
env.Alias("install", env.Install("/usr/include/lib6502", "include/lib6502/memory.h"))
env.Alias("install", env.Install("/usr/include/lib6502", "include/lib6502/makestring.h"))

Default(env["lib"])
