using namespace std;

struct monster1
{
    uint8_t type = 10;
	char name[32] = "Gordon Ramsey";
	uint8_t flag = 0xff;
	uint16_t attack = 115;
	uint16_t defense = 45;
	uint16_t regen = 30;
	int16_t health = 150;
	uint16_t gold = 75;
	uint16_t room_num = 6;
	uint16_t dlen = 70;
	string description = "A shambling British man who yell obscene things at you while working.";
}__attribute__((packed));

struct monster2
{
    uint8_t type = 10;
	char name[32] = "Bones";
	uint8_t flag = 0xff;
	uint16_t attack = 75;
	uint16_t defense = 15;
	uint16_t regen = 20;
	int16_t health = 120;
	uint16_t gold = 20;
	uint16_t room_num = 5;
	uint16_t dlen = 50;
	string description = "A short skeleton with big hands and sharp fingers";
}__attribute__((packed));

struct monster3
{
    uint8_t type = 10;
	char name[32] = "Rattles";
	uint8_t flag = 0xff;
	uint16_t attack = 70;
	uint16_t defense = 15;
	uint16_t regen = 20;
	int16_t health = 120;
	uint16_t gold = 15;
	uint16_t room_num = 5;
	uint16_t dlen = 52;
	string description = "A tall skeleton with a short sword and a nasty look";
}__attribute__((packed));

struct monster4
{
    uint8_t type = 10;
	char name[32] = "Jerry";
	uint8_t flag = 0xff;
	uint16_t attack = 70;
	uint16_t defense = 15;
	uint16_t regen = 20;
	int16_t health = 50;
	uint16_t gold = 10;
	uint16_t room_num = 5;
	uint16_t dlen = 25;
	string description = "Jerry is also a skeleton";
}__attribute__((packed));

struct monster5
{
    uint8_t type = 10;
	char name[32] = "Freddy";
	uint8_t flag = 0xff;
	uint16_t attack = 100;
	uint16_t defense = 50;
	uint16_t regen = 40;
	int16_t health = 150;
	uint16_t gold = 30;
	uint16_t room_num = 8;
	uint16_t dlen = 93;
	string description = "An ugly man with a stripped shirt, he wears claws for finger. It's definitely Freddy Kruger.";
}__attribute__((packed));

struct monster6
{
    uint8_t type = 10;
	char name[32] = "Ghost Face";
	uint8_t flag = 0xff;
	uint16_t attack = 80;
	uint16_t defense = 35;
	uint16_t regen = 30;
	int16_t health = 100;
	uint16_t gold = 50;
	uint16_t room_num = 9;
	uint16_t dlen = 44;
	string description = "It's just Ghost Face, you should be scared.";
}__attribute__((packed));

struct monster7
{
    uint8_t type = 10;
	char name[32] = "Little Jelly Guy";
	uint8_t flag = 0xff;
	uint16_t attack = 10;
	uint16_t defense = 5;
	uint16_t regen = 30;
	int16_t health = 10;
	uint16_t gold = 5;
	uint16_t room_num = 2;
	uint16_t dlen = 63;
	string description = "An adorable little jelly with big eyes is gooped on the floor.";
}__attribute__((packed));


struct monster8
{
    uint8_t type = 10;
	char name[32] = "Banker";
	uint8_t flag = 0xff;
	uint16_t attack = 200;
	uint16_t defense = 75;
	uint16_t regen = 30;
	int16_t health = 1000;
	uint16_t gold = 10000;
	uint16_t room_num = 10;
	uint16_t dlen = 52;
	string description = "Gives away free money, he knows it is useless here.";
}__attribute__((packed));


struct monster9
{
    uint8_t type = 10;
	char name[32] = "Freddy";
	uint8_t flag = 0xff;
	uint16_t attack = 60;
	uint16_t defense = 25;
	uint16_t regen = 30;
	int16_t health = 100;
	uint16_t gold;
	uint16_t room_num = 8;
	uint16_t dlen = 93;
	string description = "An ugly man with a stripped shirt, he wears claws for finger. It's definitely Freddy Kruger.";
}__attribute__((packed));

struct monster10
{
    uint8_t type = 10;
	char name[32] = "Freddy";
	uint8_t flag = 0xff;
	uint16_t attack = 100;
	uint16_t defense = 50;
	uint16_t regen = 40;
	int16_t health = 150;
	uint16_t gold;
	uint16_t room_num = 8;
	uint16_t dlen = 93;
	string description = "An ugly man with a stripped shirt, he wears claws for finger. It's definitely Freddy Kruger.";
}__attribute__((packed));