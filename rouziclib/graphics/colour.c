lrgb_t make_colour_lin(double r, double g, double b, double a)
{
	lrgb_t c;

	c.r = r * ONEF + 0.5;
	c.g = g * ONEF + 0.5;
	c.b = b * ONEF + 0.5;
	c.a = a * ONEF + 0.5;

	return c;
}

frgb_t make_colour_frgb(double r, double g, double b, double a)
{
	frgb_t c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;

	return c;
}

frgb_t make_colour_frgb_from_lrgb(lrgb_t cl)
{
	frgb_t c;
	const float ratio = 1.f/ONEF;

	c.r = (float) cl.r * ratio;
	c.g = (float) cl.g * ratio;
	c.b = (float) cl.b * ratio;
	c.a = (float) cl.a * ratio;

	return c;
}

lrgb_t make_colour_srgb(int r, int g, int b, int a)
{
	lrgb_t c;
	static int init=1;
	static lut_t slrgb_l;

	if (init)
	{
		init = 0;

		slrgb_l = get_lut_slrgb();
	}

	c.r = slrgb_l.lutint[r];
	c.g = slrgb_l.lutint[g];
	c.b = slrgb_l.lutint[b];
	c.a = slrgb_l.lutint[a];

	return c;
}

lrgb_t make_grey_lin(double v)
{
	lrgb_t c;

	c.r = v * ONEF + 0.5;
	c.g = c.r;
	c.b = c.r;
	c.a = ONE;

	return c;
}

double Lab_L_to_linear(double t)
{
	const double stn=6./29;

	t = (t+0.16) / 1.16;

	if (t > stn)
		return t*t*t;
	else
		return 3.*stn*stn*(t - 4./29.);
}

double linear_to_Lab_L(double t)
{
	const double thr = 6./29, thr3 = thr*thr*thr;

	if (t > thr3)
		t = fastpow(t, 1./3.);
	else
		t = t * 841./108. + 4./29.;

	return 1.16 * t - 0.16;
}

double Lab_L_invert(double x)
{
	return Lab_L_to_linear(1. - linear_to_Lab_L(x));
}

void rgb_to_hsl_cw(double Wr, double Wg, double Wb, double r, double g, double b, double *H, double *S, double *L, int huemode)
{
	double satv[4], cmin, cmax;		// saturated colours
	double t;
	int c1, c2, c3;

	*L = Wr*r + Wg*g + Wb*b;	// Luminosity

	cmin = MINN(r, MINN(g, b));
	cmax = MAXN(r, MAXN(g, b));

	if (cmax == cmin)		// if the input is grey
	{
		*S = 0.;
		*H = 0.;
		return ;
	}

	*S = (*L-cmin) / *L;	// Saturation

	// Fully saturate
	satv[0] = (r - *L) / *S + *L;
	satv[1] = (g - *L) / *S + *L;
	satv[2] = (b - *L) / *S + *L;
	satv[3] = satv[0];

	// find the brightest colour (c1)
	if (r==cmax)
		c1 = 0;		// red
	else if (g==cmax)
		c1 = 1;		// green
	else
		c1 = 2;		// blue

	// find the dimmest colour (c3)
	if (r==cmin)
		c3 = 0;		// red
	else if (g==cmin)
		c3 = 1;		// green
	else
		c3 = 2;		// blue

	// find the middle colour c2, even though it might be equal to c1 or c3
	c2 = 3 - c1 - c3;
	t = 1. - 0.5*linear_to_Lab_L(satv[c2]/satv[c1]);

	// if the colour is between blue and red
	if (c3==1)
	{
		if (c1==0)
			c1 = 3;		// give red a value of 3 instead of 0
		if (c2==0)
			c2 = 3;
	}

	*H = (double) c1 * t + (double) c2 * (1.-t);	// Hue

	if (huemode==HUERAD)
		*H *= (2.*pi) / 3.;
	if (huemode==HUEDEG)
		*H *= 360. / 3.;
}

void rgb_to_hsl(double r, double g, double b, double *H, double *S, double *L, int huemode)
{
	rgb_to_hsl_cw(WEIGHT_R, WEIGHT_G, WEIGHT_B, r, g, b, H, S, L, huemode);
}

void hsl_to_rgb_cw(double Wr, double Wg, double Wb, double H, double S, double L, double *r, double *g, double *b, int huemode, int secboost)
{
	double red, grn, blu, t, Y;

	// hue
	if (huemode==HUERAD)
		H *= 3. / (2.*pi);	// full circle is 3.0
	if (huemode==HUEDEG)
		H *= 3. / 360.;		// full circle is 3.0
	t = fabs(rangelimit(rangewrap(H-0., -1., 2.), -1., 1.));	red = t <= 0.5 ? 1.0 : Lab_L_to_linear(2.*(1.-t));
	t = fabs(rangelimit(rangewrap(H-1., -1., 2.), -1., 1.));	grn = t <= 0.5 ? 1.0 : Lab_L_to_linear(2.*(1.-t));
	t = fabs(rangelimit(rangewrap(H-2., -1., 2.), -1., 1.));	blu = t <= 0.5 ? 1.0 : Lab_L_to_linear(2.*(1.-t));

	// luminosity
	Y = Wr*red + Wg*grn + Wb*blu;
	Y = L/Y;	// divide by temporary lightness, multiply by desired lightness
	if (secboost)
	{
		t = 2.*fabs(rangewrap(H, 0., 1.) - 0.5);	// tc is 0 on a secondary, 1 on a primary
		Y *= (0.5*cos(pi*t)+0.5) + 1.;			// doubles lightness for secondaries
	}
	red *= Y;
	grn *= Y;
	blu *= Y;

	// saturation
	red = red*S + L*(1.-S);
	grn = grn*S + L*(1.-S);
	blu = blu*S + L*(1.-S);

	*r = red;
	*g = grn;
	*b = blu;
}

void hsl_to_rgb(double H, double S, double L, double *r, double *g, double *b, int huemode, int secboost)
{
	hsl_to_rgb_cw(WEIGHT_R, WEIGHT_G, WEIGHT_B, H, S, L, r, g, b, huemode, secboost);
}

frgb_t hsl_to_frgb(double H, double S, double L, int huemode, int secboost)
{
	double r, g, b;

	hsl_to_rgb(H, S, L, &r, &g, &b, huemode, secboost);
	return make_colour_frgb(r, g, b, 1.);
}

lrgb_t make_colour_hsl_cw(double Wr, double Wg, double Wb, double H, double S, double L, int huemode, int secboost)
{
	double r, g, b;

	hsl_to_rgb_cw(Wr, Wg, Wb, H, S, L, &r, &g, &b, huemode, secboost);

	colour_blowout_double_cw(Wr, Wg, Wb, &r, &g, &b);

	return make_colour_lin(r, g, b, 1.);
}

lrgb_t make_colour_hsl(double H, double S, double L, int huemode, int secboost)
{
	return make_colour_hsl_cw(WEIGHT_R, WEIGHT_G, WEIGHT_B, H, S, L, huemode, secboost);
}

double get_rgb_channel(lrgb_t col, int ch)
{
	double l=0.;
	const double ratio = 1./ONEF;

	switch (ch)
	{
		case 0:		// red
			l = col.r;
			break;
		case 1:		// green
			l = col.g;
			break;
		case 2:		// blue
			l = col.b;
			break;
		case 3:		// alpha
			l = col.a;
			break;
	}

	return l * ratio;
}

void lrgb_to_rgb(lrgb_t col, double *r, double *g, double *b)
{
	*r = get_rgb_channel(col, 0);
	*g = get_rgb_channel(col, 1);
	*b = get_rgb_channel(col, 2);
}

void lrgb_to_hsl(lrgb_t col, double *H, double *S, double *L, int huemode)
{
	double r, g, b;

	lrgb_to_rgb(col, &r, &g, &b);
	rgb_to_hsl(r, g, b, H, S, L, HUEDEG);
}

typedef struct
{
	double r, g, b, a;	// in 1.LBD format (as it goes up to a fp value of 1.0)
} drgb_t;			// linear RGB format

void colour_blowout_double_cw_(double Wr, double Wg, double Wb, double *pred, double *pgrn, double *pblu)
{
	int i;
	double max, L, limL, dL, d;
	double whiteL = 1. / (linear_to_Lab_L(Wr) + linear_to_Lab_L(Wg) + linear_to_Lab_L(Wb));
	drgb_t in, lim;
	
	max = MAXN(*pred, *pgrn);
	max = MAXN(max, *pblu);    // max is the maximum value of the 3 colours

	if (max > 2.)
		max = max;
	
	if (max > 1.)       // if the colour is out of gamut
	{
		in.r = *pred;	in.g = *pgrn;	in.b = *pblu;
		lim.r = MINN(in.r, 1.);
		lim.g = MINN(in.g, 1.);
		lim.b = MINN(in.b, 1.);

		L = linear_to_Lab_L(in.r*Wr) + linear_to_Lab_L(in.g*Wg) + linear_to_Lab_L(in.b*Wb);
		L *= whiteL;

		for (i=0; i<10; i++)
		{
			limL = linear_to_Lab_L(lim.r*Wr) + linear_to_Lab_L(lim.g*Wg) + linear_to_Lab_L(lim.b*Wb);
			limL *= whiteL;
			
			dL = L - limL;
			d = Lab_L_to_linear(dL);
	
			lim.r += d;
			lim.g += d;
			lim.b += d;
	
			lim.r = MINN(lim.r, 1.);
			lim.g = MINN(lim.g, 1.);
			lim.b = MINN(lim.b, 1.);
		}

		*pred = lim.r;
		*pgrn = lim.g;
		*pblu = lim.b;
	}	
}

void colour_blowout_double_cw(double Wr, double Wg, double Wb, double *pred, double *pgrn, double *pblu)
{
	double max, red, grn, blu, t, L;
	
	max = MAXN(*pred, *pgrn);
	max = MAXN(max, *pblu);    // max is the maximum value of the 3 colours
	
	if (max > 1.)       // if the colour is out of gamut
	{
		red = *pred;	grn = *pgrn;	blu = *pblu; 

		L = Wr*red + Wg*grn + Wb*blu;   // Luminosity of the colour's grey point
		
		if (L < 1.) // if the grey point is no brighter than white
		{
			// t represents the ratio on the line between the input colour
			// and its corresponding grey point. t is between 0 and 1,
			// a lower t meaning closer to the grey point and a
			// higher t meaning closer to the input colour
			t = (1.-L) / (max-L);
		
			// a simple linear interpolation between the
			// input colour and its grey point
			*pred = red*t + L*(1.-t);
			*pgrn = grn*t + L*(1.-t);
			*pblu = blu*t + L*(1.-t);
		}
		else    // if it's too bright regardless of saturation
		{
			*pred = *pgrn = *pblu = 1.;
		}
	}	
}

void colour_blowout_double(double *pred, double *pgrn, double *pblu)
{
	const double Wr=WEIGHT_R, Wg=WEIGHT_G, Wb=WEIGHT_B;         // these are the weights for each colour

	colour_blowout_double_cw(Wr, Wg, Wb, pred, pgrn, pblu);
}

void colour_blowout_int_cw(uint32_t Wr, uint32_t Wg, uint32_t Wb, uint32_t *pred, uint32_t *pgrn, uint32_t *pblu)
{
	uint32_t max, red, grn, blu, t, L;

	max = MAXN(*pred, *pgrn);
	max = MAXN(max, *pblu);    // max is the maximum value of the 3 colours
	
	if (max > ONE)       // if the colour is out of gamut
	{
		red = *pred;	grn = *pgrn;	blu = *pblu;

		// turn max from 1.LBD format to 1.15 format
		#if (LBD < 15)
			max <<= 15-LBD;
		#endif
		#if (LBD > 15)
			max >>= LBD-15;
		#endif

		L = (Wr*red + Wg*grn + Wb*blu) >> LBD;   // Luminosity of the colour's grey point, 1.15

		if (L <= 32768) // if the grey point is no brighter than white
		{
			// t represents the ratio on the line between the input colour
			// and its corresponding grey point. t is between 0 and 1,
			// a lower t meaning closer to the grey point and a
			// higher t meaning closer to the input colour
			t = ((32768-L)<<15) / (max-L);

			L = L*(32768-t) >> 15;
			// turn L from 0.15 format to 0.LBD format
			#if (LBD < 15)
				L >>= 15-LBD;
			#endif
			#if (LBD > 15)
				L <<= LBD-15;
			#endif

			// a simple linear interpolation between the
			// input colour and its grey point
			*pred = (red*t>>15) + L;
			*pgrn = (grn*t>>15) + L;
			*pblu = (blu*t>>15) + L;

			*pred = MINN(*pred, ONE);	// due to all the bit shifting, 1.0 becomes ONE+1 (e.g. 4096 instead of 4095)
			*pgrn = MINN(*pgrn, ONE);
			*pblu = MINN(*pblu, ONE);
		}
		else    // if it's too bright regardless of saturation
		{
			*pred = *pgrn = *pblu = ONE;
		}
	}
}

void colour_blowout_int(uint32_t *pred, uint32_t *pgrn, uint32_t *pblu)
{
	const uint32_t Wr=WEIGHT_R*32768.+0.5, Wg=WEIGHT_G*32768.+0.5, Wb=WEIGHT_B*32768.+0.5;        // these are the weights for each colour

	colour_blowout_int_cw(Wr, Wg, Wb, pred, pgrn, pblu);
}

void rangelimit_frgb(frgb_t *c)
{
	c->r = rangelimitf(c->r, 0., 1.);
	c->g = rangelimitf(c->g, 0., 1.);
	c->b = rangelimitf(c->b, 0., 1.);
}

lrgb_t get_colour_seq(double x, xyz_t freq, xyz_t phase)
{
	xyz_t c;

	c = add_xyz(mul_xyz(freq, set_xyz(x)), phase);
	c = add_xyz(set_xyz(0.5), mul_xyz(set_xyz(0.5), sin_xyz(mul_xyz(c, set_xyz(2.*pi)))));
	c = func1_xyz(c, Lab_L_to_linear);

	return make_colour_lin(c.x, c.y, c.z, 1.);
}
