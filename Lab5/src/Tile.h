#pragma once

#ifndef __TILE__
#define __TILE__

#include "NavigationObject.h"
#include "TileStatus.h"
#include "Label.h"
#include "NeighbourTile.h"

class Tile : public NavigationObject
{
public:

	Tile();

	~Tile();

	void draw() override;
	void update() override;
	void clean() override;


	//Constructor

	//Destructor

	//Life-Cycle Functions

	//Getters and setters
	Tile* getNeighbourTile(NeighbourTile position);
	void setNeighbourTile(NeighbourTile position, Tile* tile);

	float getTileCost() const;
	void setTileCost(float cost);

	TileStatus getTileStatus() const;
	void setTileStatus(TileStatus status); 

	void addLabels();
	void setLabelsEnabled(bool state);
private:
	//private instance members
	float m_cost;
	TileStatus m_status;

	Label* m_costLabel;
	Label* m_statusLabel;

	Tile* m_neighbours[NUM_OF_NEIGHBOUR_TILES];
};

#endif /* defined (__TILE__)*/