
OscSend xmit;
xmit.setHost("localhost", 12346);

// make HidIn and HidMsg
Hid hi;
HidMsg msg;

// which joystick
0 => int device;
// get from command line
if( me.args() ) me.arg(0) => Std.atoi => device;

// open joystick 0, exit on fail
if( !hi.openJoystick( device ) ) me.exit();

<<< "joystick '" + hi.name() + "' ready", "" >>>;

// state vars
false => int blurUP;
false => int blurDOWN;
false => int sizeUP;
false => int sizeDOWN;

false => int fractal;
false => int offset;
true => int sendSpeed;

fun void stateBased()
{
	while(true)
	{
		// send blur and blobMinSize mods
		if( blurUP )	// up
		{
			xmit.startMsg("/control/bluramt", "i");
			xmit.addInt( 2 );
		}
		if( sizeUP )	// right
		{
			xmit.startMsg("/control/blobsizemin", "i");
			xmit.addInt( 1 );
		}
		if( blurDOWN )	// down
		{
			xmit.startMsg("/control/bluramt", "i");
			xmit.addInt( -2 );
		}
		if( sizeDOWN )	// left
		{
			xmit.startMsg("/control/blobsizemin", "i");
			xmit.addInt( -1 );
		}
		40::ms => now;
	}
}

spork ~stateBased();
me.yield();

// infinite event loop
while( true )
{
    // wait on HidIn as event
    hi => now;

    // messages received
    while( hi.recv( msg ) )
    {
        // joystick button down
        if( msg.isButtonDown() )
        {
            <<< "joystick button", msg.which, "down" >>>;
			if( msg.which == 0 )
			{
				xmit.startMsg("/control/blur", "i");
				xmit.addInt( 1 );
			}
			else if( msg.which == 1 )
			{
				xmit.startMsg("/control/posterize", "i");
				xmit.addInt( 1 );
			}
			else if( msg.which == 2 )
			{
                !sendSpeed => sendSpeed;
				xmit.startMsg("/control/speed", "f");
				xmit.addFloat( 1. );
			}
			else if( msg.which == 3 )
			{
				xmit.startMsg("/control/invert", "i");
				xmit.addInt( 1 );
			}
			else if( msg.which == 4 )
			{
				true => offset;
			}
			else if( msg.which == 5 )
			{
				true => fractal;
			}
			else if( msg.which == 6 )
			{
				//xmit.startMsg("/control/setin", "i");
				//xmit.addInt( 1 );
			}
			else if( msg.which == 7 )
			{
				//xmit.startMsg("/control/setout", "i");
				//xmit.addInt( 1 );
			}
			else if( msg.which == 8 )
			{
				xmit.startMsg("/control/offsetvalue", "i");
				xmit.addInt( 0 );
				xmit.startMsg("/control/fractalamt", "i");
				xmit.addInt( 0 );
			}
			else if( msg.which == 9 )
			{
				xmit.startMsg("/control/send", "i");
				xmit.addInt( 1 );
			}
			else if( msg.which == 10 )
			{
				
			}
			else if( msg.which == 11 )
			{
				
			}
        }
        
        // joystick button up
        if( msg.isButtonUp() )
        {
            //<<< "joystick button", msg.which, "up" >>>;
			if( msg.which == 0 )
			{
				
			}
			else if( msg.which == 1 )
			{
				
			}
			else if( msg.which == 2 )
			{
				
			}
			else if( msg.which == 3 )
			{
				
			}
			else if( msg.which == 4 )
			{
				false => offset;
			}
			else if( msg.which == 5 )
			{
				false => fractal;
			}
			else if( msg.which == 6 )
			{
				
			}
			else if( msg.which == 7 )
			{
				
			}
			else if( msg.which == 8 )
			{
				
			}
			else if( msg.which == 9 )
			{
				
			}
			else if( msg.which == 10 )
			{
				
			}
			else if( msg.which == 11 )
			{
				!sendSpeed => sendSpeed;
			}
        }
        
        // joystick hat/POV switch/d-pad motion
        if( msg.isHatMotion() )
        {
            //<<< "joystick hat", msg.which, ":", msg.idata >>>;
			if( msg.idata == 0 )	// up
			{
				true => sizeUP;
			}
			else if( msg.idata == 2 )	// right
			{
				true => blurUP;
			}
			else if( msg.idata == 4 )	// down
			{
				true => sizeDOWN;
			}
			else if( msg.idata == 6 )	// left
			{
				true => blurDOWN;
			}
			// turn it all off
			else if( msg.idata == 8 )
			{
				false => blurUP => blurDOWN => sizeUP => sizeDOWN;
			}
        }
		
		// joystick axis motion
        if( msg.isAxisMotion() )
        {
			//<<< "which: ", msg.which >>>;
            //<<< "joystick axis", msg.which, ":", msg.axisPosition >>>;
			if( msg.which == 0 ) // left analog X
			{
				((msg.axisPosition + 1) * 75) $ int => int threshold;
				xmit.startMsg("/control/threshold", "i");
				xmit.addInt( threshold );
				//<<< "thr: ", threshold >>>;
			}
			if( msg.which == 1 ) // left analog Y
			{
				((msg.axisPosition + 1) * 75) $ int => int posterizeLevel;
				xmit.startMsg("/control/postlevel", "i");
				xmit.addInt( posterizeLevel );
				//<<< "pst: ", posterizeLevel >>>;
			}
			if( msg.which == 2 ) // right analog X
			{
				if( offset )
				{
					(msg.axisPosition * 50) $ int => int offsetValue;
					xmit.startMsg("/control/offsetvalue", "i");
					xmit.addInt( offsetValue );
				}
				if( fractal )
				{
					(msg.axisPosition * 50) $ int => int fractalAmt;
					xmit.startMsg("/control/fractalamt", "i");
					xmit.addInt( fractalAmt );
				}
			}
			if( msg.which == 3 ) // right analog Y
			{
				if( sendSpeed )
				{
					msg.axisPosition * 2 => float speed;
					xmit.startMsg("/control/speed", "f");
					xmit.addFloat( speed );
				}
			}
			
        }
		
    }
}
