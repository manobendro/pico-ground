int sum(int a, int b){
    return a+b;
}
int sum1(int a, int b, int c){
    return a+b+c;
}
int sum2(int a, int b, int c, int d){
    return a+b+c+d;
}
void do_sum(int *a,int *b){
    int s = sum(*a, *b);
}