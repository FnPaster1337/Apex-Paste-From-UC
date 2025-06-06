#include "../main.h"
#include "offsets.hpp"
#include <map>


__forceinline uintptr_t get_ms() { // gets the time
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount64();
	}
}

struct c_vec3 { // basic vector struc
public:
	float x, y, z;

	c_vec3(float x = 0, float y = 0, float z = 0)
		: x(x), y(y), z(z) {
	}

	inline bool operator==(const c_vec3& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	inline bool operator!=(const c_vec3& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}


	inline c_vec3& operator+=(const c_vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline c_vec3& operator-=(const c_vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline c_vec3 operator-(c_vec3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	inline c_vec3 operator+(c_vec3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	inline c_vec3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	inline c_vec3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	inline float length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	inline float distance(c_vec3 ape)
	{
		return (*this - ape).length();
	}

	/*inline c_vec3 calc_angle(const c_vec3& src, const c_vec3& dst) {
		c_vec3 angle = c_vec3();
		c_vec3 delta = c_vec3((src.x - dst.x), (src.y - dst.y), src.z - dst.z);

		double hypotenuse = sqrtf(delta.x * delta.x + delta.y * delta.y);

		angle.x = atanf(delta.z / hypotenuse) * (180.0f / M_PI);
		angle.y = atanf(delta.y / delta.x) * (180.0f / M_PI);
		angle.z = 0;
		if (delta.x >= 0.0) angle.y += 180.0f;

		return angle;
	}*/

	inline void clamp() {
		if (x > 180.0f) y = 180.0f;
		else if (y < -180.0f) y = -180.0f;

		if (x > 89.0f) x = 89.0f;
		else if (x < -89.0f) x = -89.0f;

		z = 0;
	}

	inline c_vec3 calc_angle(const c_vec3& src, const c_vec3& dst) {
		c_vec3 srca = { src.x, src.y, src.z };
		c_vec3 dsta = { dst.x, dst.y, dst.z };

		c_vec3 angle = c_vec3();
		c_vec3 delta = srca - dsta;

		double hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y));
		angle.x = (float)(atan(delta.z / hyp) * 57.295779513082f);
		angle.y = (float)(atan(delta.y / delta.x) * 57.295779513082f);
		if (delta.x >= 0.0) angle.y += 180.0f;

		return angle;
	}

	inline void normalize() {
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}

	inline bool is_zero() {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f && z > -0.1f && z < 0.1f);
	}
};

struct s_usercmd
{
	uintptr_t pVft;                // 0x00
	int32_t   m_iCmdNumber;        // 0x04
	int32_t   m_iTickCount;        // 0x08
	c_vec3 m_vecViewAngles;        // 0x0C
	c_vec3 m_vecAimDirection;      // 0x18
	float     m_flForwardmove;     // 0x24
	float     m_flSidemove;        // 0x28
	float     m_flUpmove;          // 0x2C
	int32_t   m_iButtons;          // 0x30
	uint8_t   m_bImpulse;          // 0x34
	uint8_t   Pad1[3];
	int32_t   m_iWeaponSelect;     // 0x38
	int32_t   m_iWeaponSubtype;    // 0x3C
	int32_t   m_iRandomSeed;       // 0x40
	int16_t   m_siMouseDx;         // 0x44
	int16_t   m_siMouseDy;         // 0x46
	bool      m_bHasBeenPredicted; // 0x48
	uint8_t   Pad2[27];
}; s_usercmd* g_pusercmd = nullptr;

enum cmd_buttons {
	in_attack = (1 << 0),
	in_jump = (1 << 1),
	in_duck = (1 << 2),
	in_forward = (1 << 3),
	in_back = (1 << 4),
	in_use = (1 << 5),
	in_cancel = (1 << 6),
	in_left = (1 << 7),
	in_right = (1 << 8),
	in_moveleft = (1 << 9),
	in_moveright = (1 << 10),
	in_attack2 = (1 << 11),
	in_run = (1 << 12),
	in_reload = (1 << 13),
	in_alt1 = (1 << 14),
	in_alt2 = (1 << 15),
	in_score = (1 << 16),
	in_speed = (1 << 17),
	in_walk = (1 << 18),
	in_zoom = (1 << 19),
	in_weapon1 = (1 << 20),
	in_weapon2 = (1 << 21),
	in_bullrush = (1 << 22),
	in_grenade1 = (1 << 23),
	in_grenade2 = (1 << 24),
	in_attack3 = (1 << 25)
};

struct s_globalvars {
	/*0x00*/double realtime;
	/*0x08*/int32_t framecount;
	/*0x0c*/float absoluteframetime;
	/*0x10*/float curtime;
	/*0x14*/float curtime2;
	/*0x18*/float curtime3;
	/*0x1c*/float curtime4;
	/*0x20*/float frametime;
	/*0x24*/float curtime5;
	/*0x28*/float curtime6;
	/*0x2c*/float zero;
	/*0x30*/float frametime2;
	/*0x34*/int32_t maxClients;
	/*0x38*/int32_t unk38;
	/*0x3c*/int32_t unk3C;
	/*0x40*/int32_t tickcount;
	/*0x44*/float interval_per_tick;
	/*0x48*/float interpolation_amount;
} g_pglobalvars;


struct s_matrix
{
	float matrix[16];
};

__forceinline s_matrix get_view_matrix() {
	auto view_render = read<uintptr_t>(Base + OFF_VIEWRENDER);
	if (!view_render)
		return s_matrix{};

	auto pview_matrix = read<uintptr_t>(view_render + OFF_VIEWMATRIX);
	if (pview_matrix != 0) {
		return read<s_matrix>(pview_matrix);
	}
	return s_matrix{};
}

c_vec3 world_to_screen(const struct c_vec3 pos) {
	c_vec3 to = c_vec3();
	auto m_vmatrix = get_view_matrix().matrix;

	float w = m_vmatrix[12] * pos.x + m_vmatrix[13] * pos.y + m_vmatrix[14] * pos.z + m_vmatrix[15];

	if (w < 0.01f) return c_vec3();

	to.x = m_vmatrix[0] * pos.x + m_vmatrix[1] * pos.y + m_vmatrix[2] * pos.z + m_vmatrix[3];
	to.y = m_vmatrix[4] * pos.x + m_vmatrix[5] * pos.y + m_vmatrix[6] * pos.z + m_vmatrix[7];

	float invw = 1.0f / w;
	to.x *= invw;
	to.y *= invw;

	float x = Width / 2;
	float y = Height / 2;

	x += 0.5f * to.x * Width + 0.5f;
	y -= 0.5f * to.y * Height + 0.5f;

	to.x = x;
	to.y = y;
	to.z = 0;

	return to;
}

enum weapon_ids
{
	MELEE = 18,

	SENTINEL = 1,
	CHARGE_RIFLE = 86,
	LONGBOW = 88,

	RE_45 = 85,
	P2020 = 101,
	R301 = 0,
	R99 = 100,
	ALTERNATOR = 84,

	CAR_MP = 107,
	RAMPAGE = 14,
	_30_30 = 106,
	WINGMAN = 104,
	PROWLER = 98,
	HEMLOK = 93,
	FLATLINE = 91,

	TRIPLE_TAKE = 103,
	DEVOTION = 87,
	HAVOC = 89,
	L_STAR = 95,

	MOSAMBIK = 97,
	PEACEKEEPER = 99,
	EVA_8_AUTO = 90,
	MASTIFF = 96,

	BOCEK = 2,

	KRABER = 94,
	G7_SCOUT = 92,
	VOLT = 105,
	SPITFIRE = 102
};

enum item_rarity { HEIRLOOM, LEGENDARY, EPIC, RARE, COMMON };

struct c_props {
	std::string item_name;
	item_rarity rarity;
};

std::map<uint32_t, c_props> item_map = {
	{ 1, { "Kraber", item_rarity::HEIRLOOM }},
	{ 2, { "Mastiff", item_rarity::COMMON }},
	{ 6, { "Mastiff Gold", item_rarity::LEGENDARY }},
	{ 7, { "LStar", item_rarity::COMMON }},
	{ 11, { "LStar Gold", item_rarity::LEGENDARY }},
	{ 12, { "Havoc", item_rarity::COMMON }},
	{ 16, { "Havoc Gold", item_rarity::LEGENDARY }},
	{ 17, { "Devotion", item_rarity::COMMON }},
	{ 21, { "Devotion Gold", item_rarity::LEGENDARY }},
	{ 22, { "Triple Take", item_rarity::COMMON }},
	{ 26, { "Triple Take Gold", item_rarity::LEGENDARY }},
	{ 27, { "Flatline", item_rarity::COMMON }},
	{ 31, { "Flatline Gold", item_rarity::LEGENDARY }},
	{ 32, { "Hemlock", item_rarity::COMMON }},
	{ 36, { "Hemlock Gold", item_rarity::LEGENDARY }},
	{ 37, { "G7 Scout", item_rarity::HEIRLOOM }},
	{ 41, { "G7 Scout Gold", item_rarity::LEGENDARY }},
	{ 42, { "Alternator", item_rarity::COMMON }},
	{ 46, { "Alternator", item_rarity::COMMON }},
	{ 47, { "R99", item_rarity::COMMON }},
	{ 51, { "R99 Gold", item_rarity::LEGENDARY }},
	{ 52, { "Prowler", item_rarity::COMMON }},
	{ 56, { "Prowler Gold", item_rarity::LEGENDARY }},
	{ 57, { "Volt", item_rarity::LEGENDARY }},
	{ 58, { "Longbow", item_rarity::COMMON }},
	{ 62, { "Longbow Gold", item_rarity::LEGENDARY }},
	{ 63, { "Charge Rifle", item_rarity::COMMON }},
	{ 67, { "Charge Rifle Gold", item_rarity::LEGENDARY }},
	{ 68, { "Spitfire", item_rarity::HEIRLOOM }},
	{ 73, { "R301", item_rarity::COMMON }},
	{ 69, { "R301 Gold", item_rarity::LEGENDARY }},
	{ 78, { "Eva-8 Auto", item_rarity::COMMON }},
	{ 74, { "Eva-8 Auto Gold", item_rarity::LEGENDARY }},
	{ 83, { "Peacekeeper", item_rarity::COMMON }},
	{ 79, { "Peacekeeper Gold", item_rarity::LEGENDARY }},
	{ 88, { "Mozambique", item_rarity::COMMON }},
	{ 84, { "Mozambique Gold", item_rarity::LEGENDARY }},
	{ 89, { "Wingman", item_rarity::COMMON }},
	{ 83, { "Wingman Gold", item_rarity::LEGENDARY }},
	{ 98, { "P2020", item_rarity::COMMON }},
	{ 94, { "P2020 Gold", item_rarity::LEGENDARY }},
	{ 103, { "RE45", item_rarity::COMMON }},
	{ 99, { "RE45 Gold", item_rarity::LEGENDARY }},
	{ 108, { "Sentinel", item_rarity::COMMON }},
	{ 104, { "Sentinel Gold", item_rarity::LEGENDARY }},
	{ 113, { "Bocek Bow", item_rarity::COMMON }},
	{ 109, { "Bocek Bow Gold", item_rarity::LEGENDARY }},
	{ 118, { "Repeater", item_rarity::COMMON }},
	{ 114, { "Repeater Gold", item_rarity::LEGENDARY }},
	{ 124, { "Light Ammo", item_rarity::COMMON }},
	{ 125, { "Energy Ammo", item_rarity::COMMON }},
	{ 126, { "Shotgun Ammo", item_rarity::COMMON }},
	{ 127, { "Heavy Ammo", item_rarity::COMMON }},
	{ 128, { "Sniper Ammo", item_rarity::COMMON }},
	{ 129, { "Arrow", item_rarity::COMMON }},
	{ 130, { "Rampage", item_rarity::COMMON }},
	{ 131, { "Rampage Gold", item_rarity::LEGENDARY }},
	{ 132, { "Car SMG", item_rarity::COMMON }},
	{ 133, { "Car SMG Gold", item_rarity::LEGENDARY }},
	{ 164, { "Ultimate Accelerant", item_rarity::RARE }},
	{ 165, { "Pheonix Kit", item_rarity::EPIC }},
	{ 166, { "Med Kit", item_rarity::RARE }},
	{ 167, { "Syringe", item_rarity::COMMON }},
	{ 168, { "Shield Battery", item_rarity::RARE }},
	{ 169, { "Shield Cell", item_rarity::COMMON }},
	{ 170, { "Helmet[L1]", item_rarity::COMMON }},
	{ 171, { "Helmet[L2]", item_rarity::RARE }},
	{ 172, { "Helmet[L3]", item_rarity::EPIC }},
	{ 173, { "Helmet[L4]", item_rarity::LEGENDARY }},
	{ 174, { "Body Armor[L1]", item_rarity::COMMON }},
	{ 175, { "Body Armor[L2]", item_rarity::RARE }},
	{ 176, { "Body Armor[L3]", item_rarity::EPIC }},
	{ 177, { "Body Armor[L4]", item_rarity::LEGENDARY }},
	{ 178, { "Body Armor[L5]", item_rarity::HEIRLOOM }},
	{ 179, { "Evo Body Armor[L1]", item_rarity::COMMON }},
	{ 180, { "Evo Body Armor[L2]", item_rarity::RARE }},
	{ 181, { "Evo Body Armor[L3]", item_rarity::EPIC }},
	{ 182, { "Evo Body Armor[L5]", item_rarity::HEIRLOOM }},
	{ 237, { "Heat Shield", item_rarity::HEIRLOOM }},
	{ 184, { "Knockdown Shield[L1]", item_rarity::COMMON }},
	{ 185, { "Knockdown Shield[L2]", item_rarity::RARE }},
	{ 186, { "Knockdown Shield[L3]", item_rarity::EPIC }},
	{ 187, { "Knockdown Shield[L4]", item_rarity::LEGENDARY }},
	{ 188, { "Backpack[L1]", item_rarity::COMMON }},
	{ 189, { "Backpack[L2]", item_rarity::RARE }},
	{ 190, { "Backpack[L3]", item_rarity::EPIC }},
	{ 191, { "Backpack[L4]", item_rarity::LEGENDARY }},
	{ 192, { "Thermite Grenade", item_rarity::COMMON }},
	{ 193, { "Frag Grenade", item_rarity::COMMON }},
	{ 194, { "Arc Star", item_rarity::COMMON }},
	{ 195, { "1x HCOG 'Classic'", item_rarity::COMMON }},
	{ 196, { "2x HCOG 'Bruiser'", item_rarity::RARE }},
	{ 197, { "1x Holo", item_rarity::COMMON }},
	{ 198, { "1x-2x Variable Holo", item_rarity::RARE }},
	{ 199, { "1x Digital Threat", item_rarity::LEGENDARY }},
	{ 200, { "3x HCOG 'Ranger'", item_rarity::EPIC }},
	{ 201, { "2x-4x Variable AOG", item_rarity::EPIC }},
	{ 202, { "6x Sniper", item_rarity::RARE }},
	{ 203, { "4x-8x Variable Sniper", item_rarity::EPIC }},
	{ 204, { "4x-10x Digital Sniper Threat", item_rarity::LEGENDARY }},
	{ 205, { "Barrel Stabilizer", item_rarity::COMMON }},
	{ 206, { "Barrel Stabilizer", item_rarity::RARE }},
	{ 207, { "Barrel Stabilizer", item_rarity::EPIC }},
	{ 209, { "Light Magazine[L1]", item_rarity::COMMON }},
	{ 210, { "Light Magazine[L2]", item_rarity::RARE }},
	{ 211, { "Light Magazine[L3]", item_rarity::EPIC }},
	{ 212, { "Light Magazine[L4]", item_rarity::LEGENDARY }},
	{ 213, { "Heavy Magazine[L1]", item_rarity::COMMON }},
	{ 214, { "Heavy Magazine[L2]", item_rarity::RARE }},
	{ 215, { "Heavy Magazine[L3]", item_rarity::EPIC }},
	{ 216, { "Heavy Magazine[L4]", item_rarity::LEGENDARY }},
	{ 217, { "Energy Magazine[L1]", item_rarity::COMMON }},
	{ 218, { "Energy Magazine[L2]", item_rarity::RARE }},
	{ 219, { "Energy Magazine[L3]", item_rarity::EPIC }},
	{ 220, { "Energy Magazine[L4]", item_rarity::LEGENDARY }},
	{ 221, { "Sniper Magazine[L1]", item_rarity::COMMON }},
	{ 222, { "Sniper Magazine[L2]", item_rarity::RARE }},
	{ 223, { "Sniper Magazine[L3]", item_rarity::EPIC }},
	{ 224, { "Sniper Magazine[L4]", item_rarity::LEGENDARY }},
	{ 225, { "Shotgun Bolt[L1]", item_rarity::COMMON }},
	{ 226, { "Shotgun Bolt[L2]", item_rarity::RARE }},
	{ 227, { "Shotgun Bolt[L3]", item_rarity::EPIC }},
	{ 228, { "Standard Stock[L1]", item_rarity::COMMON }},
	{ 229, { "Standard Stock[L2]", item_rarity::RARE }},
	{ 230, { "Standard Stock[L3]", item_rarity::EPIC }},
	{ 231, { "Sniper Stock[L1]", item_rarity::COMMON }},
	{ 232, { "Sniper Stock[L2]", item_rarity::RARE }},
	{ 233, { "Sniper Stock[L3]", item_rarity::EPIC }},
	{ 234, { "Turbocharger", item_rarity::LEGENDARY }},
	{ 238, { "Hammer Point", item_rarity::EPIC }},
	{ 237, { "Skull Piercer", item_rarity::LEGENDARY } },
	{ 245, { "Deadeye's Tempo", item_rarity::EPIC } },
	{ 246, { "Quickdraw Holster", item_rarity::EPIC } },
	{ 247, { "Shatter Caps", item_rarity::EPIC }},
	{ 248, { "Kinetic Feeder", item_rarity::EPIC } },
	{ 249, { "Boosted Loader", item_rarity::LEGENDARY }},
	{ 245, { "Mobile Respawn Beacon", item_rarity::RARE }},
	{ 246, { "Vault Key", item_rarity::HEIRLOOM }},
	{ 248, { "Treasure Pack", item_rarity::HEIRLOOM }},
	{ 249, { "Survival Doubler", item_rarity::HEIRLOOM }}, // TODO: Find this rarity
	{ 250, { "Survival Quadrupler", item_rarity::HEIRLOOM }}, // TODO: Find this rarity
	{ 251, { "Heat Shield", item_rarity::RARE }},
	{ 252, { "Mobile Respawn Beacon 2", item_rarity::RARE }},
	{ 253, { "Treasure Pack", item_rarity::HEIRLOOM }},
	{ 254, { "Vault Key", item_rarity::HEIRLOOM }}
};

std::map<uint32_t, std::string> weapon_map = {
	{ 1, { "kraber", item_rarity::HEIRLOOM }},
};

class c_weapon {
public:
	c_weapon() = default;
	c_weapon(uintptr_t pointer) {
		this->address = pointer;
	}

	inline bool valid() {
		return this->address != 0;
	}

	inline int weapon_id() {
		if (!this->address) return -1;
		return read<int>(this->address + OFF_WEAPON_HANDLE);
	}

	inline int ammo_in_clip() {
		if (!this->address) return -1;
		return read<int>(this->address + OFF_WEAPON_AMMO);
	}

	inline float bullet_speed() {
		if (!this->address) return 0.0f;
		return read<float>(this->address + OFF_PROJECTILESPEED);
	}

	inline float bullet_gravity() {
		if (!this->address) return 0.0f;
		return read<float>(this->address + OFF_PROJECTILESCALE);
	}

	
	inline std::string get_name() {
		if (!this->address) return ("none");
		for (const auto& [key, value] : weapon_map) {
			if (this->weapon_id() == key) {
				return value;
			}
		}
	}
private:
	uintptr_t address;
};

struct s_glow
{
	int8_t general, border, border_size, transparency;
};

struct s_visible_time
{
	float last_time_visible[100]; // last visible time in float
	bool last_visibility_state[100]; // last visibility state
	uintptr_t last_check_time[100]; // last visibility check time
} inline last_visible;

struct s_box { // experimental
	s_box() = default;
	s_box(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
	int x, y, w, h;
};

class c_player {

public:
	c_player() = default;
	c_player(DWORD_PTR pointer) {
		this->address = pointer;
		this->health = this->i_health();
		this->shield = this->i_shield();
		this->team = this->i_team();
		this->origin = this->vec_origin();
		this->bleed_out_state = read<int>(pointer + OFF_BLEEDOUT_STATE);
	}

	inline bool operator!=(const c_player& player) const {
		return (this->address != player.address);
	}

	inline uintptr_t get_address() {
		return this->address;
	}

	inline bool valid() {
		if (this->vec_origin() != c_vec3(0,0,0) && this->address != NULL && this->alive())
			return true;
		return false;
	}

	inline bool b_downed() {
		if (this->bleed_out_state < 1) {
			return true;
		}
		return false;
	}

	inline c_weapon weapon() {
		auto weapon_pointer = read<uintptr_t>(this->address + OFF_WEAPON_HANDLE);
		weapon_pointer &= 0xffff;
		auto weapon_handle = read<uintptr_t>(Base + OFF_ENTITY_LIST + (weapon_pointer << 5));
		return c_weapon(weapon_handle);
	}

	inline std::string s_name() {
		auto index = read<uintptr_t>(this->address + OFF_NAME_INDEX);
		auto offsets = read<uintptr_t>((Base + OFF_NAME_LIST) + ((index - 1) << 4));
		char buf[64] = { 0 };
		SofMain::ReadPhysicalMemory((PVOID)offsets, &buf, 64);
		return buf;
	}

	inline int i_health() {
		this->health = read<int>(this->address + OFF_HEALTH);
		return this->health;
	}

	inline bool on_ground() {
		this->flags = read<uint32_t>(this->address + OFF_FLAGS);
		return (this->flags & 0x1) != 0;
	}

	inline bool is_ducking() {
		this->flags = read<uint32_t>(this->address + OFF_FLAGS);
		return (this->flags & 0x2) != 0;
	}

	inline bool alive() {
		return read<int>(this->address + OFF_BLEEDOUT_STATE) == 0 && read<int>(this->address + OFF_LIFE_STATE) == 0;
	}

	inline int i_team() {
		this->team = read<int>(this->address + OFF_TEAM_NUMBER);
		return this->team;
	}

	inline int i_shield() {
		this->shield = read<int>(this->address + OFF_SHIELD);
		return this->shield;
	}

	inline int i_max_health() {
		this->max_health = i_health() + i_shield();
		return this->max_health;
	}


	inline c_vec3 mins() {
		return read<c_vec3>(this->address + OFF_Collision_m_vecMins);
	}

	inline c_vec3 maxs() {
		return read<c_vec3>(this->address + OFF_Collision_m_vecMaxs);
	}

	inline int distance() {
		auto local_player = read<uintptr_t>(Base + OFF_LOCAL_PLAYER);
		auto local_dist = read<c_vec3>(local_player + OFF_LOCAL_ORIGIN);

		return (int)local_dist.distance(this->vec_origin());
	}

	inline c_vec3 vec_origin() {
		this->origin = read<c_vec3>(this->address + OFF_LOCAL_ORIGIN);
		return this->origin;
	}

	inline c_vec3 view_angles() {
		return read<c_vec3>(this->address + OFF_VIEW_ANGLES);
	}

	inline c_vec3 camera_position() {
		return read<c_vec3>(this->address + OFF_CAMERAORIGIN);
	}

	inline c_vec3 bone_position(uint32_t id) { // should use hitboxes instead of bones for skeleton esp
		uintptr_t bone_array = read<uintptr_t>(this->address + OFF_BONES);
		this->origin = this->vec_origin();

		c_vec3 bone = c_vec3();
		bone.x = read<float>(bone_array + 0xCC + (id * 0x30)) + this->origin.x;
		bone.y = read<float>(bone_array + 0xDC + (id * 0x30)) + this->origin.y;
		bone.z = read<float>(bone_array + 0xEC + (id * 0x30)) + this->origin.z;
		return bone;
	}

	inline bool visible(int index) {
		if (get_ms() >= (last_visible.last_check_time[index] + 10)) {
			float s_visible_time = read<float>(this->address + OFF_LAST_VISIBLE_TIME);
			last_visible.last_visibility_state[index] = s_visible_time > last_visible.last_time_visible[index] || s_visible_time < 0.f && last_visible.last_time_visible[index] > 0.f;
			last_visible.last_time_visible[index] = s_visible_time;
			last_visible.last_check_time[index] = get_ms();
		}
		return last_visible.last_visibility_state[index];
	}

	// localplayer only	
	inline c_vec3 punch_angles() {
		return read<c_vec3>(this->address + OFF_PUNCH_ANGLES);
	}

	inline void view_angles(c_vec3 angles) {
		write<c_vec3>(this->address + OFF_VIEW_ANGLES, angles);
	}


private:
	DWORD_PTR address;
	int health;
	int shield;
	int max_health;
	int team;
	int bleed_out_state;
	int diving;
	c_vec3 origin;
	uint32_t flags;
};

__forceinline std::string class_signifier(DWORD_PTR entity) {
	auto class_pointer = read<uintptr_t>(entity + OFF_SignifierName);
	char buffer[128] = { 0 };
	SofMain::ReadPhysicalMemory((PVOID)class_pointer, &buffer, 64);
	return buffer;
}

void draw_text(float x, float y, ImColor color, const char* string, ...) {
	char buf[512];
	va_list arg_list;

	ZeroMemory(buf, sizeof(buf));

	va_start(arg_list, string);
	vsnprintf(buf, sizeof(buf), string, arg_list);
	va_end(arg_list);

	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(x, y), color, buf, 0, 0, 0);
	return;
}

void draw_line(float x, float y, float x2, float y2, ImColor color, float thickness) {
	if (x < 0)
		return;

	if (y < 0)
		return;

	if (x2 > (float)Width)
		return;

	if (y2 > (float)Height)
		return;

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x2, y2), color, thickness);
	return;
}

void draw_rectangle(float x, float y, float x2, float y2, ImColor color, float thickness) {
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + x2, y + y2), color, 0, -1, thickness);
	return;
}

void draw_cornered_box(int x, int y, int w, int h, const ImColor color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + (w / 3), y), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, thickness);
}


void draw_rectangle_filled(float x, float y, float x2, float y2, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + x2, y + y2), color);
	return;
}

void draw_rectangle_filled_multicolor(float x, float y, float x2, float y2, ImColor color, ImColor color2, ImColor color3, ImColor color4) {
	ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + x2, y + y2), color, color2, color3, color4);
	return;
}

void draw_corner_box(float x, float y, float x2, float y2, ImColor color, float thickness) {
	float box_width = x2 - x;
	float box_height = y2 - y;

	// top
	draw_line(x, y, x + box_width / 4, y, color, thickness);
	draw_line(x2, y, x2 - box_width / 4, y, color, thickness);

	// left
	draw_line(x, y, x, y + box_height / 3, color, thickness);
	draw_line(x, y2, x, y2 - box_height / 3, color, thickness);

	// right
	draw_line(x2, y, x2, y + box_height / 3, color, thickness);
	draw_line(x2, y2, x2, y2 - box_height / 3, color, thickness);

	// bottom
	draw_line(x, y2, x + box_width / 4, y2, color, thickness);
	draw_line(x2, y2, x2 - box_width / 4, y2, color, thickness);

	return;
}

void draw_circle(float x, float y, ImColor color, float radius) {
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color);
	return;
}

void draw_filled_circle(float x, float y, ImColor color, float radius) {
	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, color);
	return;
}

s_box create_box(c_player player) { // used for dynamic boxes and esp
	c_vec3 origin{}, min{}, max{}, flb{}, brt{}, blb{}, frt{}, frb{}, brb{}, blt{}, flt{};
	int left, top, right, bottom;

	origin = player.vec_origin();
	min = player.mins() + origin;
	max = player.maxs() + origin;

	c_vec3 points[] = {
	  c_vec3(min.x, min.y, min.z),
	  c_vec3(min.x, max.y, min.z),
	  c_vec3(max.x, max.y, min.z),
	  c_vec3(max.x, min.y, min.z),
	  c_vec3(max.x, max.y, max.z),
	  c_vec3(min.x, max.y, max.z),
	  c_vec3(min.x, min.y, max.z),
	  c_vec3(max.x, min.y, max.z)
	};

	flb = world_to_screen(points[3]);
	brt = world_to_screen(points[5]);
	blb = world_to_screen(points[0]);
	frt = world_to_screen(points[4]);
	frb = world_to_screen(points[2]);
	brb = world_to_screen(points[1]);
	blt = world_to_screen(points[6]);
	flt = world_to_screen(points[7]);

	c_vec3 arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	if (left || top || right || bottom) {
		for (auto x = 1; x < 8; ++x) { // 8 different points
			if (left > arr[x].x)
				left = arr[x].x;
			if (bottom < arr[x].y)
				bottom = arr[x].y;
			if (right < arr[x].x)
				right = arr[x].x;
			if (top > arr[x].y)
				top = arr[x].y;
		}
		return { (int)left, (int)top, int(right - left), int(bottom - top) };
	}
} // experimental