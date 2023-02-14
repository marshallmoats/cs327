
enum CharType {
    Player,
    Hiker,
    Rival,
    Pacer,
    Wanderer,
    Sentry,
    Explorer,
    Swimmer,
};


struct Character {
    int x, y;
    enum CharType type;
};