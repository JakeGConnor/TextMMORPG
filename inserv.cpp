/*
 * 
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
#include <vector>
#include <time.h>
#include "Rooms.h"
#include "Monsters.h"
using namespace std;

int skt;

mutex clients_mutex;

void cleanup_clients();

class version
{
public:
	// You don't have to use uint8_t, it's the same as "unsigned char"
	uint8_t type = 14;
	uint8_t major;
	uint8_t minor;
	uint16_t extension_length = 0;
	version(uint8_t p_major, uint8_t p_minor) : major(p_major), minor(p_minor) {}
	bool send(int skt)
	{
		return 5 == write(skt, this, 5);
	}
} __attribute__((packed)); // we'll have to make sure this is packed

class game
{
public:
	uint8_t type = 11;
	uint16_t initial_points;
	uint16_t stat_limit;
	uint16_t description_length;

	string description;
	game(uint16_t ip, uint16_t sl, string d) : initial_points(ip), stat_limit(sl), description(d) {}
	bool send(int skt)
	{
		description_length = description.length();
		if (7 != write(skt, this, 7))
			return false;
		if (description_length != write(skt, description.c_str(), description_length))
			return false;
		return true;
	}
} __attribute__((packed));

void shut_down_server(int signal)
{
	printf("\nDoing everything we need to do to shut down the server\n");
	close(skt);
	exit(0);
}

struct flag_byte
{
	uint8_t reserved : 3;
	uint8_t ready : 1;
	uint8_t started : 1;
	uint8_t monster : 1;
	uint8_t join_battle : 1;
	uint8_t alive : 1;
} __attribute__((packed));

struct character
{
	uint8_t type = 10;
	char name[32];
	uint8_t flag;
	uint16_t attack;
	uint16_t defense;
	uint16_t regen;
	int16_t health;
	uint16_t gold;
	uint16_t room_num;
	uint16_t dlen;
	char *description;
	uint8_t unlock = 0;
} __attribute__((packed));

struct message
{
	uint8_t type = 1;
	uint16_t mlen;
	char recipient[32];
	char sender[32];
	char *message;
} __attribute__((packed));

struct accept
{
	uint8_t type = 8;
	uint8_t type_accpted;
} __attribute__((packed));

struct reject
{
	uint8_t type = 7;
	uint8_t error_code;
	uint16_t err_len;
	string err_msg;
} __attribute__((packed));

struct connect
{
	uint8_t type = 13;
	uint16_t room_num;
	char name[32];
	uint16_t dlen;
	string description;
} __attribute__((packed));

class client
{
public:
	int fd;
	string address;
	mutex write_mutex;
	thread t;
	struct character ch;
	void run();
	static void start_own_thread(client *us)
	{
		us->run();
	}
	client(int our_fd, string a) : fd(our_fd), t(start_own_thread, this), address(a) {}
} __attribute__((packed));

vector<client *> clients;

void cleanup_clients()
{
	clients_mutex.lock();
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i]->fd == -1)
		{
			// delete that client
			clients[i]->t.join();
			delete clients[i];
			clients[i] = clients[clients.size() - 1];
			clients.pop_back();
		}
	}
	clients_mutex.unlock();
}

struct message m;
struct monster1 m1;
struct monster2 m2;
struct monster3 m3;
struct monster4 m4;
struct monster5 m5;
struct monster6 m6;
struct monster7 m7;
struct monster8 m8;
struct monster9 m9;
struct monster10 m10;

void client::run()
{
	// This is where handle_client goes

	bool reject = true;
	printf("is this running\n");

	while (1)
	{
	here:
		// struct character ch;
		struct room1 rm1;
		struct room2 rm2;
		struct room3 rm3;
		struct room4 rm4;
		struct room5 rm5;
		struct room6 rm6;
		struct room7 rm7;
		struct room8 rm8;
		struct room9 rm9;
		struct room10 rm10;
		struct reject rjct;
		struct accept acpt;
		struct flag_byte fb;
		uint8_t connection = 13;
		uint8_t lootT = 5;

		uint8_t type;
		recv(fd, &type, 1, MSG_WAITALL);
		// printf("test: %u\n", type);
		if (type == 0)
		{
		}
		else if (type == 1) // MESSAGE
		{
			clients_mutex.lock();
			recv(fd, &m.mlen, 66, MSG_WAITALL);
			m.message = (char *)malloc(m.mlen + 1);
			recv(fd, m.message, m.mlen, MSG_WAITALL);

			for (int i = 0; i < clients.size(); i++)
			{
				if (strcmp(clients[i]->ch.name, m.recipient) == 0)
				{
					write_mutex.lock();
					write(clients[i]->fd, &m.type, 1);
					write(clients[i]->fd, &m.mlen, 2);
					write(clients[i]->fd, m.recipient, 32);
					write(clients[i]->fd, m.sender, 32);
					write(clients[i]->fd, m.message, m.mlen);
					write_mutex.unlock();
					clients_mutex.unlock();
					goto here;
				}
			}

			clients_mutex.unlock();
			write_mutex.lock();
			rjct.error_code = 6;
			rjct.err_msg = "\nNo target.\n";
			rjct.err_len = rjct.err_msg.length();
			write(fd, &rjct, 4);
			write(fd, rjct.err_msg.c_str(), rjct.err_len);
			write_mutex.unlock();
		}
		else if (type == 2) // CHANGEROOM
		{
			int old_room = ch.room_num;
			recv(fd, &ch.room_num, sizeof(uint16_t), MSG_WAITALL);
			int new_room = ch.room_num;

			if (old_room == ch.room_num)
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 1 && (ch.room_num != 2 && ch.room_num != 10))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 2 && (ch.room_num != 1 && ch.room_num != 3 && ch.room_num != 4))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 3 && (ch.room_num != 2 && ch.room_num != 6))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 4 && (ch.room_num != 2 && ch.room_num != 5))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 5 && ch.room_num != 4)
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 6 && (ch.room_num != 3 && ch.room_num != 7 && ch.room_num != 8 && ch.room_num != 9))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 7 && (ch.room_num != 6 && ch.room_num != 8 && ch.room_num != 9))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 8 && (ch.room_num != 7))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else if (old_room == 9 && (ch.room_num != 7 && ch.room_num != 6))
			{
				write_mutex.lock();
				rjct.error_code = 1;
				rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
				ch.room_num = old_room;
			}
			else
			{
				write_mutex.lock();
				acpt.type_accpted = 2;
				write(fd, &acpt, 2);

				write(fd, &ch, 48);
				write(fd, ch.description, ch.dlen);

				write_mutex.unlock();
				reject = false;

				if (new_room == 1)
				{
					write_mutex.lock();
					write(fd, &rm1.type, 1);
					write(fd, &rm1.num, 36);
					write(fd, rm1.description.c_str(), rm1.dlen);

					if (ch.unlock == 1)
					{
						write(fd, &connection, 1);
						write(fd, &rm10.num, 36);
						write(fd, rm10.description.c_str(), rm10.dlen);
					}

					write(fd, &connection, 1);
					write(fd, &rm2.num, 36);
					write(fd, rm2.description.c_str(), rm2.dlen);
					write_mutex.unlock();
				}
				else if (new_room == 2)
				{
					write_mutex.lock();
					write(fd, &rm2.type, 1);
					write(fd, &rm2.num, 36);
					write(fd, rm2.description.c_str(), rm2.dlen);

					write(fd, &connection, 1);
					write(fd, &rm1.num, 36);
					write(fd, rm1.description.c_str(), rm1.dlen);

					write(fd, &connection, 1);
					write(fd, &rm3.num, 36);
					write(fd, rm3.description.c_str(), rm3.dlen);

					write(fd, &connection, 1);
					write(fd, &rm4.num, 36);
					write(fd, rm4.description.c_str(), rm4.dlen);

					write(fd, &m7, 48);
					write(fd, m7.description.c_str(), m7.dlen);

					write_mutex.unlock();
				}

				else if (new_room == 3)
				{
					write_mutex.lock();
					write(fd, &rm3.type, 1);
					write(fd, &rm3.num, 36);
					write(fd, rm3.description.c_str(), rm3.dlen);

					write(fd, &connection, 1);
					write(fd, &rm2.num, 36);
					write(fd, rm2.description.c_str(), rm2.dlen);

					write(fd, &connection, 1);
					write(fd, &rm6.num, 36);
					write(fd, rm6.description.c_str(), rm6.dlen);
					write_mutex.unlock();
				}

				else if (new_room == 4)
				{
					write_mutex.lock();
					write(fd, &rm4.type, 1);
					write(fd, &rm4.num, 36);
					write(fd, rm4.description.c_str(), rm4.dlen);

					write(fd, &connection, 1);
					write(fd, &rm2.num, 36);
					write(fd, rm2.description.c_str(), rm2.dlen);

					write(fd, &connection, 1);
					write(fd, &rm5.num, 36);
					write(fd, rm5.description.c_str(), rm5.dlen);
					write_mutex.unlock();
				}
				else if (new_room == 5)
				{
					write_mutex.lock();
					write(fd, &rm5.type, 1);
					write(fd, &rm5.num, 36);
					write(fd, rm5.description.c_str(), rm5.dlen);

					write(fd, &connection, 1);
					write(fd, &rm4.num, 36);
					write(fd, rm4.description.c_str(), rm4.dlen);

					write(fd, &m2, 48);
					write(fd, m2.description.c_str(), m2.dlen);

					write(fd, &m3, 48);
					write(fd, m3.description.c_str(), m3.dlen);

					write(fd, &m4, 48);
					write(fd, m4.description.c_str(), m4.dlen);

					write_mutex.unlock();
				}
				else if (new_room == 6)
				{
					write_mutex.lock();
					write(fd, &rm6.type, 1);
					write(fd, &rm6.num, 36);
					write(fd, rm6.description.c_str(), rm6.dlen);

					write(fd, &connection, 1);
					write(fd, &rm3.num, 36);
					write(fd, rm3.description.c_str(), rm3.dlen);

					write(fd, &connection, 1);
					write(fd, &rm7.num, 36);
					write(fd, rm7.description.c_str(), rm7.dlen);

					write(fd, &connection, 1);
					write(fd, &rm8.num, 36);
					write(fd, rm8.description.c_str(), rm8.dlen);

					write(fd, &connection, 1);
					write(fd, &rm9.num, 36);
					write(fd, rm9.description.c_str(), rm9.dlen);

					write(fd, &m1, 48);
					write(fd, m1.description.c_str(), m1.dlen);

					write_mutex.unlock();
				}
				else if (new_room == 7)
				{
					write_mutex.lock();
					write(fd, &rm7.type, 1);
					write(fd, &rm7.num, 36);
					write(fd, rm7.description.c_str(), rm7.dlen);

					write(fd, &connection, 1);
					write(fd, &rm6.num, 36);
					write(fd, rm6.description.c_str(), rm6.dlen);

					write(fd, &connection, 1);
					write(fd, &rm8.num, 36);
					write(fd, rm8.description.c_str(), rm8.dlen);

					write(fd, &connection, 1);
					write(fd, &rm9.num, 36);
					write(fd, rm9.description.c_str(), rm9.dlen);

					write_mutex.unlock();
				}
				else if (new_room == 8)
				{
					write_mutex.lock();
					write(fd, &rm8.type, 1);
					write(fd, &rm8.num, 36);
					write(fd, rm8.description.c_str(), rm8.dlen);

					write(fd, &connection, 1);
					write(fd, &rm7.num, 36);
					write(fd, rm7.description.c_str(), rm7.dlen);

					write(fd, &m5, 48);
					write(fd, m5.description.c_str(), m5.dlen);

					write_mutex.unlock();
				}
				else if (new_room == 9)
				{
					write_mutex.lock();
					write(fd, &rm9.type, 1);
					write(fd, &rm9.num, 36);
					write(fd, rm9.description.c_str(), rm9.dlen);

					write(fd, &connection, 1);
					write(fd, &rm7.num, 36);
					write(fd, rm7.description.c_str(), rm7.dlen);

					write(fd, &connection, 1);
					write(fd, &rm6.num, 36);
					write(fd, rm6.description.c_str(), rm6.dlen);

					write(fd, &m6, 48);
					write(fd, m6.description.c_str(), m6.dlen);

					ch.unlock = 1;

					write_mutex.unlock();
				}
				else if (new_room == 10)
				{
					if (ch.unlock == 1)
					{
						write_mutex.lock();
						write(fd, &rm10.type, 1);
						write(fd, &rm10.num, 36);
						write(fd, rm10.description.c_str(), rm10.dlen);

						write(fd, &connection, 1);
						write(fd, &rm1.num, 36);
						write(fd, rm1.description.c_str(), rm1.dlen);

						write(fd, &m8, 48);
						write(fd, m8.description.c_str(), m8.dlen);

						write_mutex.unlock();
					}
					else
					{
						write_mutex.lock();
						rjct.error_code = 1;
						rjct.err_msg = "\nBad room. Attempt to change to an inappropriate room\n";
						rjct.err_len = rjct.err_msg.length();
						write(fd, &rjct, 4);
						write(fd, rjct.err_msg.c_str(), rjct.err_len);
						write_mutex.unlock();
					}
				}

				clients_mutex.lock();

				for (int i = 0; i < clients.size(); i++)
				{
					if (clients[i]->ch.room_num == new_room)
					{
						write_mutex.lock();
						write(clients[i]->fd, &ch, 48);
						write(clients[i]->fd, ch.description, ch.dlen);

						write(fd, &clients[i]->ch.type, 1);
						write(fd, &clients[i]->ch.name, 47);
						write(fd, &clients[i]->ch.description, clients[i]->ch.dlen);
						write_mutex.unlock();
					}
					if (clients[i]->ch.room_num == old_room)
					{
						write_mutex.lock();
						write(clients[i]->fd, &ch, 48);
						write(clients[i]->fd, ch.description, ch.dlen);

						write(fd, &clients[i]->ch.type, 1);
						write(fd, &clients[i]->ch.name, 47);
						write(fd, &clients[i]->ch.description, clients[i]->ch.dlen);
						write_mutex.unlock();
					}
				}
				clients_mutex.unlock();
			}
		}
		else if (type == 3) // FIGHT
		{
			if (ch.room_num == 2)
			{
				if (m7.flag & 0x80)
				{
					int damage_pc = m7.attack - ch.defense;
					int damage_mon = ch.attack - m7.defense;

					if (damage_pc <= 0)
					{
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					else
					{
						ch.health = ch.health - damage_pc;
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					if (damage_mon <= 0)
					{
						write_mutex.lock();
						write(fd, &m7, 48);
						write(fd, m7.description.c_str(), m7.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m7.health = m7.health - damage_mon;
						if (m7.health <= 0)
						{
							m7.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();

							for (int i = 0; i < clients.size(); i++)
							{
								if (clients[i]->ch.room_num == ch.room_num)
								{
									write_mutex.lock();
									write(clients[i]->fd, &m7, 48);
									write(clients[i]->fd, m7.description.c_str(), m7.dlen);
									write_mutex.unlock();
								}
							}
						}
						clients_mutex.unlock();
					}
				}
			}
			else if (ch.room_num == 5)
			{
				if (m2.flag & 0x80 && m3.flag & 0x80)
				{
					int damage_pc = m3.attack + m2.attack - ch.defense;
					int damage_m2 = ch.attack - m2.defense;
					int damage_m3 = ch.attack - m3.defense;

					if (damage_pc <= 0)
					{
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					else
					{
						ch.health = ch.health - damage_pc;
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					if (damage_m2 <= 0)
					{
						write_mutex.lock();
						write(fd, &m2, 48);
						write(fd, m2.description.c_str(), m2.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m2.health = m2.health - damage_m2;
						if (m2.health <= 0)
						{
							m2.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();
						}
						else
						{
							m2.health = m2.health + m2.regen;
						}
						for (int i = 0; i < clients.size(); i++)
						{
							if (clients[i]->ch.room_num == ch.room_num)
							{
								write_mutex.lock();
								write(clients[i]->fd, &m2, 48);
								write(clients[i]->fd, m2.description.c_str(), m2.dlen);

								write_mutex.unlock();
							}
						}
						clients_mutex.unlock();
					}
					if (damage_m3 <= 0)
					{
						write_mutex.lock();
						write(fd, &m3, 48);
						write(fd, m3.description.c_str(), m3.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m3.health = m3.health - damage_m3;
						if (m3.health <= 0)
						{
							m3.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();
						}
						else
						{
							m3.health = m3.health + m3.regen;
						}
						for (int i = 0; i < clients.size(); i++)
						{
							if (clients[i]->ch.room_num == ch.room_num)
							{
								write_mutex.lock();
								write(clients[i]->fd, &m3, 48);
								write(clients[i]->fd, m3.description.c_str(), m3.dlen);

								write_mutex.unlock();
							}
						}
						clients_mutex.unlock();
					}
				}
			}
			else if (ch.room_num == 6)
			{
				if (m1.flag & 0x80)
				{
					int damage_pc = m1.attack - ch.defense;
					int damage_mon = ch.attack - m1.defense;

					if (damage_pc <= 0)
					{
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					else
					{
						ch.health = ch.health - damage_pc;
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					if (damage_mon <= 0)
					{
						write_mutex.lock();
						write(fd, &m1, 48);
						write(fd, m1.description.c_str(), m1.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m1.health = m1.health - damage_mon;
						if (m1.health <= 0)
						{
							m1.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();
						}
						else
						{
							m1.health = m1.health + m1.regen;
						}

						for (int i = 0; i < clients.size(); i++)
						{
							if (clients[i]->ch.room_num == ch.room_num)
							{
								write_mutex.lock();
								write(clients[i]->fd, &m1, 48);
								write(clients[i]->fd, m1.description.c_str(), m1.dlen);

								write_mutex.unlock();
							}
						}
						clients_mutex.unlock();
					}
				}
			}
			else if (ch.room_num == 8)
			{
				if (m5.flag & 0x80)
				{
					int damage_pc = m5.attack - ch.defense;
					int damage_mon = ch.attack - m5.defense;

					if (damage_pc <= 0)
					{
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					else
					{
						ch.health = ch.health - damage_pc;
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					if (damage_mon <= 0)
					{
						write_mutex.lock();
						write(fd, &m5, 48);
						write(fd, m5.description.c_str(), m5.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m5.health = m5.health - damage_mon;
						if (m1.health <= 0)
						{
							m5.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();
						}
						else
						{
							m5.health = m5.health + m5.regen;
						}
						for (int i = 0; i < clients.size(); i++)
						{
							if (clients[i]->ch.room_num == ch.room_num)
							{
								write_mutex.lock();
								write(clients[i]->fd, &m5, 48);
								write(clients[i]->fd, m5.description.c_str(), m5.dlen);

								write_mutex.unlock();
							}
						}
						clients_mutex.unlock();
					}
				}
			}
			else if (ch.room_num == 9)
			{
				if (m6.flag & 0x80)
				{
					int damage_pc = m6.attack - ch.defense;
					int damage_mon = ch.attack - m6.defense;

					if (damage_pc <= 0)
					{
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					else
					{
						ch.health = ch.health - damage_pc;
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					if (damage_mon <= 0)
					{
						write_mutex.lock();
						write(fd, &m6, 48);
						write(fd, m6.description.c_str(), m6.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m6.health = m6.health - damage_mon;
						if (m6.health <= 0)
						{
							m6.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();
						}
						else
						{
							m6.health = m6.health + m6.regen;
						}
						for (int i = 0; i < clients.size(); i++)
						{
							if (clients[i]->ch.room_num == ch.room_num)
							{
								write_mutex.lock();
								write(clients[i]->fd, &m6, 48);
								write(clients[i]->fd, m6.description.c_str(), m6.dlen);

								write_mutex.unlock();
							}
						}
						clients_mutex.unlock();
					}
				}
			}
			else if (ch.room_num == 10)
			{
				if (m6.flag & 0x80)
				{
					int damage_pc = m8.attack - ch.defense;
					int damage_mon = ch.attack - m8.defense;

					if (damage_pc <= 0)
					{
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					else
					{
						ch.health = ch.health - damage_pc;
						write_mutex.lock();
						write(fd, &ch, 48);
						write(fd, ch.description, ch.dlen);
						write_mutex.unlock();
					}
					if (damage_mon <= 0)
					{
						write_mutex.lock();
						write(fd, &m8, 48);
						write(fd, m8.description.c_str(), m8.dlen);
						write_mutex.unlock();
					}
					else
					{
						clients_mutex.lock();
						m8.health = m8.health - damage_mon;
						if (m6.health <= 0)
						{
							m8.flag &= (~0x80);
							write_mutex.lock();
							write(fd, &ch, 48);
							write(fd, ch.description, ch.dlen);
							write_mutex.unlock();
						}
						else
						{
							m8.health = m8.health + m8.regen;
						}
						for (int i = 0; i < clients.size(); i++)
						{
							if (clients[i]->ch.room_num == ch.room_num)
							{
								write_mutex.lock();
								write(clients[i]->fd, &m8, 48);
								write(clients[i]->fd, m8.description.c_str(), m8.dlen);

								write_mutex.unlock();
							}
						}
						clients_mutex.unlock();
					}
				}
			}
			else
			{
				write_mutex.lock();
				rjct.error_code = 7;
				rjct.err_msg = "\nNo fight. Sent if the requested fight cannot happen for other reasons (i.e. no live monsters in room)\n";
				rjct.err_len = rjct.err_msg.length();
				write(fd, &rjct, 4);
				write(fd, rjct.err_msg.c_str(), rjct.err_len);
				write_mutex.unlock();
			}
			if (ch.health <= 0)
			{
				ch.flag &= (~0x80);
				write_mutex.lock();
				write(fd, &ch, 48);
				write(fd, ch.description, ch.dlen);
				write_mutex.unlock();
			}
			else if (ch.health < 200)
			{
				ch.health = ch.health + ch.regen;
				if (ch.health > 200)
				{
					ch.health = 200;
				}
				write_mutex.lock();
				write(fd, &ch, 48);
				write(fd, ch.description, ch.dlen);
				write_mutex.unlock();
			}
		}
		else if (type == 4) // PVP
		{
			write_mutex.lock();
			rjct.error_code = 8;
			rjct.err_msg = "\nPVP is not currently supported\n";
			rjct.err_len = rjct.err_msg.length();
			write(fd, &rjct, 4);
			write(fd, rjct.err_msg.c_str(), rjct.err_len);
			write_mutex.unlock();
		}
		else if (type == 5)
		{
			int loot;
			srand(time(NULL));
			loot = rand() % 10 + 1;

			ch.gold = ch.gold + loot;

			write_mutex.lock();
			write(fd, &lootT, 1);
			write(fd, ch.name, 32);
			write(fd, &ch, 48);
			write(fd, ch.description, ch.dlen);
			write_mutex.unlock();
		}
		else if (type == 6)
		{
			//ch.flag &= (0x08);

			write_mutex.lock();
			write(fd, &ch, 48);
			write(fd, ch.description, ch.dlen);

			uint8_t tmp = 13;
			write(fd, &rm1.type, 1);
			write(fd, &rm1.num, 36);
			write(fd, rm1.description.c_str(), rm1.dlen);
			write_mutex.unlock();
		}
		else if (type == 10) // CHARACTER
		{
			while (reject)
			{
				ch.type = 10;
				recv(fd, &ch.name, 47, MSG_WAITALL);
				ch.description = (char *)malloc(ch.dlen + 1);
				recv(fd, ch.description, ch.dlen, MSG_WAITALL);

				uint8_t flags = ch.flag;
				ch.room_num = 1;
				ch.health = 200;
				ch.gold = 0;

				printf("Flags:  %x\n", flags);

				ch.flag &= (~0x20);

				if (flags & 0x80)
					printf("Alive:  True\n");
				else
					printf("Alive:  False\n");

				printf("Join Battle:  %s\n", (flags & 0x40) ? "True" : "False");
				printf("Monster:  %s\n", (flags & 0x20) ? "True" : "False");
				printf("Started:  %s\n", (flags & 0x10) ? "True" : "False");
				printf("Ready:  %s\n", (flags & 0x08) ? "True" : "False");

				printf("sizeof(struct flag_byte) = %lu\n", sizeof(struct flag_byte)); // size is 1 byte
				memcpy(&fb, &flags, 1);												  // Because C++ didn't like fb = flags;
				if (fb.alive)														  // Same as above, we just use fb.alive instead of flags & 0x80
					printf("Alive:  True\n");
				else
					printf("Alive:  False\n");

				printf("Name:  %s\n", ch.name);
				printf("Attack:  %d\n", ch.attack);
				printf("Defence:  %d\n", ch.defense);
				printf("Regen:  %d\n", ch.regen);
				printf("Health:  %d\n", ch.health);
				printf("Gold:  %d\n", ch.gold);
				printf("Current Room:  %d\n", ch.room_num);
				printf("Description Length:  %d\n", ch.dlen);
				printf("Description:  %s\n", ch.description);

				uint16_t stats = ch.attack + ch.defense + ch.regen;
				if (stats <= 200)
				{
					printf("this\n");
					acpt.type_accpted = 10;
					write_mutex.lock();

					write(fd, &acpt, 2);

					write(fd, &ch, 48);
					write(fd, ch.description, ch.dlen);

					write(fd, &rm1.type, 1);
					write(fd, &rm1.num, 36);
					write(fd, rm1.description.c_str(), rm1.dlen);

					write(fd, &connection, 1);
					write(fd, &rm2.num, 36);
					write(fd, rm2.description.c_str(), rm2.dlen);
					write_mutex.unlock();
					reject = false;
				}
				else
				{
					write_mutex.lock();
					rjct.error_code = 4;
					rjct.err_msg = "\nCharacter Stats are too high, Please try again!\n";
					rjct.err_len = rjct.err_msg.length();
					write(fd, &rjct, 4);
					write(fd, rjct.err_msg.c_str(), rjct.err_len);
					write_mutex.unlock();
					recv(fd, &type, 1, MSG_WAITALL);
				}
			}
			reject = true;
		}
	}

	// write(c->fd, ((char*)&sm) + 10, sizeof(sm) - 10);
	/* Remember to unlock!  (very important!) */

	clients_mutex.unlock();
	printf("Done with client %d(from address %s)\n", fd, address.c_str());
	// it'd be cool if we actually freed up this place in(c->fds
	close(fd);
	fd = -1;
}

int main(int argc, char **argv)
{
	uint16_t listen_port = 5005; // 5005 is my default
	if (argc > 1)
	{
		listen_port = atoi(argv[1]);
		if (listen_port < 1)
		{
			printf("Invalid port:  %s\n", argv[1]);
			exit(1);
		}
		printf("Will listen on port %d\n", listen_port);
	}
	else
	{
		printf("No port specified, defaulting to %d\n", listen_port);
	}
	struct sigaction sa;
	sa.sa_handler = shut_down_server;
	sigaction(SIGINT, &sa, 0);

	struct sockaddr_in sad;
	sad.sin_port = htons(listen_port);
	sad.sin_addr.s_addr = INADDR_ANY;
	sad.sin_family = AF_INET;

	skt = socket(AF_INET, SOCK_STREAM, 0); // Step 1
	if (skt == -1)
	{
		perror("socket");
		return 1;
	}
	if (bind(skt, (struct sockaddr *)(&sad), sizeof(struct sockaddr_in)))
	{ // step 2
		perror("bind");
		return 1;
	}
	if (listen(skt, 5))
	{ // step 3
		perror("listen");
		return 1;
	}
	int client_fd;
	struct sockaddr_in client_address;
	socklen_t address_size = sizeof(struct sockaddr_in);
	version our_version(2, 3);
	game our_game(200, 3880, "The mortal world is no longer where you make your moves. \nYou fall into a pit of fire after your mortal death and now must traverse through a hellish mansion!");

	while (1)
	{
		clients_mutex.unlock();
		client_fd = accept(skt, (struct sockaddr *)(&client_address), &address_size); // step 4
		printf("Connection made from address %s\n", inet_ntoa(client_address.sin_addr));

		our_version.send(client_fd);
		our_game.send(client_fd);

		clients_mutex.lock();
		clients.push_back(new client(client_fd, inet_ntoa(client_address.sin_addr)));
		clients_mutex.unlock();
		printf("Currently we have %lu clients\n", clients.size());
		cleanup_clients();
	}
	return 0;
}
