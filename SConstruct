import os

env = Environment(ENV = os.environ)
VariantDir('build', '.')

SRC = [
'build/src/ali-emu.cpp',
'build/src/colors.cpp',
'build/src/events.cpp',
]

env.ParseConfig('pkg-config --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5')
env.Program('ali-emu', SRC)
