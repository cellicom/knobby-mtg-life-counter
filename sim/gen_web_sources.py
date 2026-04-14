import os
import sys

def rwildcard(directory, pattern):
    sources = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(pattern):
                sources.append(os.path.join(root, file).replace('\\', '/'))
    return sources

def main():
    if len(sys.argv) < 3:
        print("Usage: gen_web_sources.py <lvgl_path> <knobby_path>")
        sys.exit(1)
        
    lvgl_path = sys.argv[1]
    knobby_path = sys.argv[2]
    
    sources = [
        'sim_sdl2.c',
        'sim_stubs.c'
    ]
    
    # Knobby sources
    knobby_files = [
        'knob.c', 'src/ui_1p.c', 'src/ui_mp.c', 'src/settings.c',
        'src/intro.c', 'src/timer.c', 'src/game.c', 'src/damage_log.c',
        'src/game_mode.c', 'src/rename.c', 'src/dice.c', 'src/hw.c', 'src/storage.c'
    ]
    sources.extend([os.path.join(knobby_path, f).replace('\\', '/') for f in knobby_files])
    
    # Fonts
    fonts_dir = os.path.join(knobby_path, 'src/fonts')
    if os.path.exists(fonts_dir):
        sources.extend(rwildcard(fonts_dir, '.c'))
        
    # LVGL
    sources.extend(rwildcard(os.path.join(lvgl_path, 'src'), '.c'))
    
    with open('web_sources.txt', 'w') as f:
        f.write(' '.join(sources))

if __name__ == '__main__':
    main()
