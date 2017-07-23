import pygame.mixer
from pygame.mixer import Sound
from gpiozero import Button
from signal import pause

pygame.mixer.init()
button_sounds = {
	Button(18): Sound("Music/c_note.wav"),
	Button(23): Sound("Music/d_flat_note.wav"),
	Button(24): Sound("Music/d_note.wav"),
	Button(25): Sound("Music/e_flat_note.wav"),
	Button(17): Sound("Music/e_note.wav"),
	Button(27): Sound("Music/f_note.wav"),
	Button(22): Sound("Music/g_flat_note.wav"),
	Button(2): Sound("Music/g_note.wav"),
	Button(3): Sound("Music/a_flat_note.wav"),
	Button(4): Sound("Music/a_note.wav"),
	Button(11): Sound("Music/b_flat_note.wav"),
	Button(15): Sound("Music/b_note.wav"),
}
for button, sound in button_sounds.items():
	button.when_pressed = sound.play