using namespace std;

struct room1
{
	uint8_t type = 9;
	uint16_t num = 1;
	char name[32] = "The Gate";
	uint16_t dlen = 63;
	string description = "Welcome to the gates of hell, where all your dreams come true!";
} __attribute__((packed));

struct room2
{
	uint8_t type = 9;
	uint16_t num = 2;
	char name[32] = "Hell's Foyer";
	uint16_t dlen = 68;
	string description = "A strangly tastefully decorated room, though most of it is on fire.";
} __attribute__((packed));

struct room3
{
	uint8_t type = 9;
	uint16_t num = 3;
	char name[32] = "The Abyss";
	uint16_t dlen = 87;
	string description = "A black void sits at the edge of the room, you feel it pulling your soul in. Great fun!";
} __attribute__((packed));

struct room4
{
	uint8_t type = 9;
	uint16_t num = 4;
	char name[32] = "Infinite Hallway";
	uint16_t dlen = 94;
	string description = "Looking to the end you see a large door that never gets closer, but there is one on the right.";
} __attribute__((packed));

struct room5
{
	uint8_t type = 9;
	uint16_t num = 5;
	char name[32] = "Dungeon";
	uint16_t dlen = 68;
	string description = "Skeletons line the dark stone walls, they are moving very slightly.";
} __attribute__((packed));

struct room6
{
	uint8_t type = 9;
	uint16_t num = 6;
	char name[32] = "Hell's Kitchen";
	uint16_t dlen = 94;
	string description = "Firey stoves line the walls and dark stainless countertops house all the ingredients to pain.";
} __attribute__((packed));
// connections 3 7 8 9
struct room7
{
	uint8_t type = 9;
	uint16_t num = 7;
	char name[32] = "Dying Room";
	uint16_t dlen = 120;
	string description = "A burnt sectional and coffee table sits in the middle of the room, kind of cozy, but the TV is looping annoying orange.";
} __attribute__((packed));
// connections 6 8 9 
struct room8
{
	uint8_t type = 9;
	uint16_t num = 8;
	char name[32] = "Nightmare Bedroom";
	uint16_t dlen = 134;
	string description = "A clean room with a large 4 post bed, looks very comfortable. A weird looking guy with a striped turtle neck is in the corner though.";
} __attribute__((packed));
// connections 7
struct room9
{
	uint8_t type = 9;
	uint16_t num = 9;
	char name[32] = "Bathroom from Scream";
	uint16_t dlen = 72;
	string description = "Honestly a normal looking bathroom, other than the toilet looks hungry.";
} __attribute__((packed));
// connections 7

struct room10
{
	uint8_t type = 9;
	uint16_t num = 10;
	char name[32] = "Treasure Room";
	uint16_t dlen = 72;
	string description = "A room full of gold.";
}__attribute__((packed));