
int cread_alt(int *xp)
{
        int t = 0;
        return *(xp ? xp : &t);
}