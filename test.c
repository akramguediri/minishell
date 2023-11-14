#include <ncurses.h>

int	main(void)
{
	int	ch;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	printw("Press Arrow Up, Arrow Down, or Delete. Press q to quit.\n");
	refresh();
	while ((ch = getch()) != 'q')
	{
		if (ch == KEY_UP)
		{
			printw("Up Arrow Pressed\n");
		}
		else if (ch == KEY_DOWN)
		{
			printw("Down Arrow Pressed\n");
		}
		else if (ch == 127)
		{
			printw("Delete Key Pressed\n");
		}
		refresh();
	}
	endwin();
	return (0);
}
