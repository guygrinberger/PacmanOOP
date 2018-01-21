#include "Sounds.h"

Sounds::Sounds()
{
	// create all sounds
	buffer.loadFromFile("buttom.ogg");
	buttom.setBuffer(buffer);
	buffer1.loadFromFile("welcome.ogg");
	welcome.setBuffer(buffer1);
	buffer2.loadFromFile("bye.wav");
	bye.setBuffer(buffer2);
	buffer3.loadFromFile("eat.ogg");
	eat.setBuffer(buffer3);
	Background.openFromFile("Background_sound.ogg");
	Background.play();
	Background.setLoop(true);
}

Sounds::~Sounds()
{
}
