#include "Vertex.h"

float Vertex::gravity = 0.1f;

Vertex::Vertex(sf::Vector2f passedPosition) {
	position = passedPosition;
	oldPosition = passedPosition;

	vertShape.setFillColor(sf::Color::Magenta);
	vertShape.setRadius(5.f);
	vertShape.setOrigin(vertShape.getOrigin().x + vertShape.getRadius(), vertShape.getOrigin().y + vertShape.getRadius());
	vertShape.setPosition(position);

}

void Vertex::updateVertexPosition() {
	float dx = position.x - oldPosition.x;
	float dy = position.y - oldPosition.y;

	oldPosition = position;
	position.x += dx;
	position.y += dy;
	position.y += gravity;
}

void Vertex::constrainVertexPosition(int height, int width) {
	float dx = position.x - oldPosition.x;
	float dy = position.y - oldPosition.y;

	// If the particle crosses a border, there are two useful distances to track: B shows how far beyond the border the particle has gone; A shows how much it covered before reaching the border. Both the position and the oldPosition need to be adjusted based on such collisions, to set up for the subsequent frame.
	float A, B;
	if (position.x > width) {
		A = width - position.x;
		B = position.x + dx - width;
		oldPosition.x = position.x + 2 * A;
		position.x = width - B;
	}
	else if (position.x < 0) {
		A = -position.x;
		B = dx - A;
		oldPosition.x = position.x + 2 * A;
		position.x = -B;
	}

	if (position.y > height) {
		A = height - position.y;
		B = position.y + dy - height - gravity;
		oldPosition.y = position.y + 2 * A;
		position.y = height - B;
	}
	else if (position.y < 0) {
		A = -position.y;
		B = dy - A + gravity;
		oldPosition.y = position.y + 2 * A;
		position.y = -B;
	}
}