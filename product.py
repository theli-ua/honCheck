import itertools
lists = [
    ['#define FOG_TYPE 0','#define FOG_TYPE 1'],
    ['#define FOG_OF_WAR 0','#define FOG_OF_WAR 1'],
    ['#define FOG_QUALITY 1','#define FOG_QUALITY 0'],
    ['#define FALLOFF_QUALITY 1','#define FALLOFF_QUALITY 0'],
]
for element in itertools.product(*lists):
    print '"'+'\\n'.join(element)+'\\n",'
