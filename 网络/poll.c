static int do_poll(unsigned int nfds, struct poll_list *list, struct poll_wqueues *wait,

                   long timeout)
{
    int count = 0;
    poll_table *pt = &wait->pt;
    for (;;) //一直循环
    {
        struct poll_list *walk;
        set_current_state(TASK_INTERRUPTIBLE);
        walk = list;
        while (walk != NULL)
        {
            do_pollfd(walk->len, walk->entries, &pt, &count);
            walk = walk->next;
        }
        pt = NULL;
        if (count || !timeout || signal_pending(current))
            break;
        count = wait->error;
        if (count)
            break;
        timeout = schedule_timeout(timeout); /* 让current挂起，别的进程跑，timeout到了
以后再回来运行current*/
    }
    __set_current_state(TASK_RUNNING);
    return count;
}