class value_network{
private:
    int size;
    int **board;
    int ***network;
public:
    value_network(int s, int **arr);
    int pattern(int arr[5], int player);
    int *estimate();
    bool compare_arr(int arr1[5], int arr2[5]);
    void refresh(int **arr);
    void printout();
    void init_network();
};