#include <u.h>
#include <libc.h>

void
usage(void)
{
	fprint(2, "usage: %s [-m map]\n", argv0);
	exits("usage");
}

enum {
	Left = 0x01, Mid = 0x02, Right = 0x04,
	Scrld = 0x08, Scrlu = 0x10,
	Fwd = 0x20, Bwd = 0x40,
	Fn1 = 0x80, Fn2 = 0x100, Fn3 = 0x200,
};

typedef struct Butt Butt;
struct Butt {
	int raw; int cooked;
	int down;
	void (*action)(void);
};

Butt identity[] = {
	{Left, Left, 0, nil},
	{Mid, Mid, 0, nil},
	{Right, Right, 0, nil},
	{Scrld, Scrld, 0, nil},
	{Scrlu, Scrlu, 0, nil},
	{-1, -1, -1, nil},
};

Butt huge[] = {
	{Left, Left, 0, nil},
	{Mid, Mid, 0, nil},
	{Right, Mid, 0, nil},
	{Scrld, Scrld, 0, nil},
	{Scrlu, Scrlu, 0, nil},
	{Fwd, Mid, 0, nil},
	{Bwd, Left, 0, nil},
	{Fn1, Mid, 0, nil},
	{Fn2, Right, 0, nil},
	{Fn3, Right, 0, nil},
	{-1, -1, -1, nil},
};

Butt contour[] = {
	{Left, Left, 0, nil},
	{Mid, Mid, 0, nil},
	{Right, Right, 0, nil},
	{Scrld, Scrld, 0, nil},
	{Scrlu, Scrlu, 0, nil},
	{Fwd, Scrld, 0, nil},
	{Bwd, Scrlu, 0, nil},
	{-1, -1, -1, nil},
};

int
parsein(int *x, int *y, int *b)
{
	int nr;
	char buf[1+12+12+12+1];
	char *p;
	
	if((nr = read(0, buf, sizeof buf)) < 0)
		sysfatal("read: %r");
	if(nr == 0)
		return 0;
	buf[nr] = 0;
	
	*x = strtol(buf+1, &p, 10);
	if(p == nil)
		sysfatal("x not a num");

	*y = strtol(p, &p, 10);
	if(p == nil)
		sysfatal("y not a num");

	*b = strtol(p, &p, 10);
	if(p == nil)
		sysfatal("b not a num");
	
	return nr;
}

static int
buttonmap(Butt *m, int b)
{
	int r;
	
	for(r = 0; m->raw != -1; m++){
		if(m->raw & b){
			if(m->action != nil && m->down == 0)
				m->action();
			r |= m->cooked;
			m->down = 1;
		}else
			m->down = 0;
	}
	return r;
}

void
main(int argc, char **argv)
{
	int x, y, b;
	char *mapn;
	Butt *map;

	mapn = "default";
	ARGBEGIN{
	case 'm':
		mapn = EARGF(usage());
		break;
	default:
		usage();
	}ARGEND;

	map = identity;
	if(strncmp(mapn, "elecom", 6) == 0
	|| strncmp(mapn, "huge", 4) == 0)
		map = huge;
	else if(strncmp(mapn, "contour", 7) == 0)
		map = contour;
	
	x = y = b = 0;
	for(;;){
		if(parsein(&x, &y, &b) == 0)
			break;
		// x = scale(s, x);
		// y = scale(s, y);
		b = buttonmap(map, b);

		fprint(1, "%s%11d %11d %11d", "m", x, y, b);
	}
	exits(nil);
}
