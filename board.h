class board {
private:
    int board_size;
    int **board_array;
public:
    board(int size);
    void init();
    int **get_board();
    void print();
    bool move(int x, int y, int player);
    char *verdict(int player);
};