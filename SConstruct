env = Environment()

sources = [
    "cpu.cpp",
    "branch.cpp",
    "flags.cpp",
    "stack.cpp",
    "ora.cpp",
    "lda.cpp",
    "sta.cpp",
    "sbc.cpp",
    "cmp.cpp",
    "asl.cpp",
    "and.cpp",
    "adc.cpp",
    "eor.cpp",
    "cpx.cpp",
    "cpy.cpp",
    "ror.cpp",
    "ldy.cpp",
    "sty.cpp",
    "lsr.cpp",
    "rol.cpp",
    "inc.cpp",
    "dec.cpp",
    "ldx.cpp",
    "stx.cpp"
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
