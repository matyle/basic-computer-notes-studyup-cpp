#pragma D option flowindent

syscall::fsync:entry
/execname == "io" && guard++ == 0/
{
    self->traceme = 1;
}

fbt:::
/self->traceme/
{}

syscall::fsync:return
/self->traceme/
{
    self->traceme = 0;
    exit(0);
}
