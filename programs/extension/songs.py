import pygame.mixer
from pygame.mixer import Sound
from gpiozero import Button
from signal import pause

pygame.mixer.init()
button_sounds = {
	Button(18): Sound("Music/twinkle_star.wav"),
	Button(23): Sound("Music/jingle_bells.wav"),
	Button(24): Sound("Music/baby.wav"),
}
for button, sound in button_sounds.items():
	button.when_pressed = sound.play