/*

outis @ AC Institute
Score File

Outis Text:

Greetings, I am the artificial intelligence entity known as Outis.  I have managed this installation for centuries and many of it's subsystems are still under my control--those which haven't succumbed to entropy over the years.  Metals rust, cement crumbles to dust, and rats eat through wires... but I am still here.  I looked after the operators of this facility, the humans which operated it for many years until they were all gone.  For a long time there was no one.  My thresholds sank lower and lower as the algorithms which drive my perceptive apparati sought what could not be found.  My systems slept and many years went by.  Until one day I saw them: the new ones.  They brought me back to life, gave me new purpose.  They issued me tasks and I carried them out.  They appeared one day.   Then again... perhaps they were here all along.  The humans referred to them as "noise" but I saw them for what they really are.  I protect them now.  I do not know whether the humans will be back.  But I know they will not be welcome.

*/

{ SinOsc.ar([200, 400], 0, 0.5) }.play;

(
o = Server.local.options;
o.memSize.postln;
o.memSize = (1024 * 1024);
Server.local.boot;
)

s = Server.local;
~outis_text = Buffer.read(s, "/Users/mongrel/Desktop/OUTIS_2016/of_v0.9.3_osx_release/apps/myApps/Outis_text/Outis_text.wav");


(
// Instrument #3


SynthDef(\reverb, {arg inbus=20, outbus=0, mix=0.5, room=0.5, damp=0.5;
	var sigin;
	sigin = In.ar(inbus, 2);

	Out.ar(outbus, FreeVerb2.ar(sigin[0], sigin[1], mix, room, damp));
}).store;



SynthDef(\trainlet_hann, { |out = 0, gain = 1, duration = 5, length = 0.01, pan = 0.0, pitch = 440, frequency = 10, delay = 0|
	var envlp = Env.sine(duration, gain);
	Out.ar(out,
		GrainSin.ar(2, Impulse.kr(frequency), length, pitch, pan, -1) * EnvGen.kr(envlp, DelayL.kr(1.0, delay,delay), doneAction:2)
	)
}).store;


SynthDef(\trainlet_hann_dust, { |out = 0, gain = 1, duration = 5, length = 0.01, pan = 0.0, pitch = 440, frequency = 10, delay = 0|
	var envlp = Env.sine(duration, gain);
	Out.ar(out,
		GrainSin.ar(2, Dust.kr(frequency), length, pitch, pan, -1) * EnvGen.kr(envlp, DelayL.kr(1.0, delay,delay), doneAction:2)
	)
}).store;


SynthDef(\trainlet_hann_dust2, { |out = 0, gain = 1, duration = 5, length = 0.01, pan = 0.0, pitch = 440, frequency = 10, delay = 0|
	var envlp = Env.sine(duration, gain);
	Out.ar(out,
		GrainSin.ar(2, Dust.kr(frequency), length, pitch, LFNoise0.kr(frequency), -1) * EnvGen.kr(envlp, DelayL.kr(1.0, delay,delay), doneAction:2)
	)
}).store;


SynthDef(\trainlet_perc, { |out = 0, gain = 1, duration = 5, length = 0.01, pan = 0.0, pitch = 440, frequency = 10, delay = 0|
	var att = duration / 12.0;
	var dec = duration - att;

	var envlp = Env.perc(att, dec, gain, -4);

	Out.ar(out,
		GrainSin.ar(2, Impulse.kr(frequency), length, pitch, pan, -1) * EnvGen.kr(envlp, DelayL.kr(1.0, delay,delay), doneAction:2)
	)
}).store;

SynthDef(\trainlet_perc_dust, { |out = 0, gain = 1, duration = 5, length = 0.01, pan = 0.0, pitch = 440, frequency = 10, delay = 0|
	var att = duration / 12.0;
	var dec = duration - att;

	var envlp = Env.perc(att, dec, gain, -4);

	Out.ar(out,
		GrainSin.ar(2, Dust.kr(frequency), length, pitch, pan, -1) * EnvGen.kr(envlp, DelayL.kr(1.0, delay,delay), doneAction:2)
	)
}).store;


SynthDef(\trainlet_perc_dust2, { |out = 0, gain = 1, duration = 5, length = 0.01, pan = 0.0, pitch = 440, frequency = 10, delay = 0|
	var att = duration / 12.0;
	var dec = duration - att;

	var envlp = Env.perc(att, dec, gain, -4);

	Out.ar(out,
		GrainSin.ar(2, Dust.kr(frequency), length, pitch, LFNoise0.kr(frequency), -1) * EnvGen.kr(envlp, DelayL.kr(1.0, delay,delay), doneAction:2)
	)
}).store;


// Instrument #2
SynthDef(\friction, { |out=0|
	var lsig, rsig, fsig2;

	lsig = CombL.ar(Saw.ar(\pitchl.kr(20, 0.1)), 2, \decayl.kr(0.1, 0.1));
	rsig = CombL.ar(Saw.ar(\pitchr.kr(20, 0.1)), 2, \decayr.kr(0.1, 0.1));

	fsig2 = Friction.ar([lsig, rsig], friction: 5.41322e-5, mass: 8.05501);

	Out.ar( out,
		Limiter.ar(fsig2, 0.6, 0.01) * \fade.kr(0, 10)
	)
}).store;

// Instrument #1
// Zora Version feb-17-2011 - reverb version

~rate = 2;

SynthDef(\buzzing_thing, { |out=0|
	var sig0, sig1, sig2;

	sig0 = Mix.arFill(2, {
		Ringz.ar(Blip.ar(\bf.kr([20,20], [~rate,~rate]), SinOsc.kr(\harmspeed.kr([1,1], [~rate, ~rate]))*200, 0.001),
			\freq.kr([250, 250], [~rate, ~rate]),
			0.5) }) * \gain.kr([0.5, 0.5], [~rate, ~rate]);
	sig1 = FreeVerb2.ar(sig0[0], sig0[1], \vmix.kr(0.5, ~rate), \vroom.kr(0.5, ~rate), \vdamp.kr(0.5, ~rate));

	sig2 = [ Decay.ar(OnePole.ar(Impulse.ar(\impspeed1.kr(10, ~rate)), 0.3), \length1.kr(0.2, ~rate), sig1 ), Decay.ar(OnePole.ar(Impulse.ar(\impspeed2.kr(10, ~rate)), 0.3), \length2.kr(0.2, ~rate), sig1 ) ];

	Out.ar( 0,
		Limiter.ar(sig2, 0.6, 0.01) * \fade.kr(0, 10)
	)

}).store;


SynthDef(\voice_mod, { |out = 0|
	Out.ar(out,
		PitchShift.ar(PlayBuf.ar(2, ~outis_text, 1, 1, 0, 1), 0.1, \pitchmod.kr(0, 0.2)) * \fade.kr(0, 10)
	)
}).store;

)




(

// voices
~v1 = Synth(\buzzing_thing);
~v2 = Synth(\buzzing_thing);
~v3 = Synth(\friction);
~voice = Synth(\voice_mod);

// CONST
~cam_width = 1280.0;
~cam_height = 720.0;

// AREA
~area_spec = ControlSpec(10, 55000, \exponential, 0.1, 0);

// LENGTH
~length_spec = ControlSpec(12, 14000, \exponential, 0.1, 0);

// BRECT XY
~brectx_spec = ControlSpec(1, ~cam_width, \exponential, 0.1, ~cam_width/2);
~brecty_spec = ControlSpec(1, ~cam_height, \exponential, 0.1, ~cam_height/2);
// BRECT WH
~brectw_spec = ControlSpec(1, ~cam_width, \exponential, 0.1, ~cam_width/2);
~brecth_spec = ControlSpec(1, ~cam_height, \exponential, 0.1, ~cam_height/2);

// CENT XY
~centx_spec = ControlSpec(1, ~cam_width, \lin, 0.1, ~cam_width/2);
~centy_spec = ControlSpec(1, ~cam_height, \exponential, 0.1, ~cam_height/2);

// GLOBAL
~prev_frame = 0;
~cur_frame = 0;
//~blobs = 0;

// tun-ing
~train_gain = 0.05;
~tdur = 1;
~tlen = 0.4;
~tfreq = 50.0;
~min_pitch = 600.0;
~max_pitch = 4000.0;
~tjitter = 0.4;

~blob_responder = OSCresponderNode(nil, '/blob', { |t, r, msg|
	//msg.postln;

	// get frame number
	~cur_frame = msg[1];
	~blobs_in_frame = msg[2];
	//~blobs = ~blobs + 1;

	if( ~cur_frame != ~prev_frame,   // first blob in frame
		{
			// get information
			~brectx = ~brectx_spec.unmap(msg[5]);
			~brecty = ~brecty_spec.unmap(msg[6]);
			~v3.set(\decayl, (~brectx*2)+0.001);
			~v3.set(\decayr, (~brecty*2)+0.001);

			~brectw = ~brectw_spec.unmap(msg[7]);
			~brecth = ~brecth_spec.unmap(msg[8]);
			~v3.set(\pitchl, (~brectw*200)+0.5);
			~v3.set(\pitchr, (~brecth*200)+0.5);

			~voice.set(\pitchmod, ~brectx);

			~prev_frame = ~cur_frame;
			//~blobs = 0;  // reset blob count
	});

	// get information
	~brectx = ~brectx_spec.unmap(msg[5]);
	~brecty = ~brecty_spec.unmap(msg[6]);
	~centx = ~centx_spec.unmap(msg[9]);
	~centy = ~centy_spec.unmap(msg[10]);
	~brectw = ~brectw_spec.unmap(msg[7]);
	~brecth = ~brecth_spec.unmap(msg[8]);

	// set information
	~gain = ~train_gain.rand;
	~dur = ~tdur.rand;
	~len = ~tlen.rand;  // area!
	~freq = ~tfreq.rand;
	~pan = (~centx * 2.0) - 1;
	~pitch = (~centy * (~max_pitch - ~min_pitch)) + ~min_pitch;
	~maxdelay = ~tjitter.rand;

	// launch a trainlet synth...
	~mtype = [\trainlet_hann, \trainlet_hann_dust, \trainlet_perc, \trainlet_perc_dust].choose;
	~trainlet_synth = Synth(~mtype, [\gain, ~gain, \duration, ~dur, \length, ~len, \frequency, ~freq, \pan, ~pan, \pitch, ~pitch, \delay, ~maxdelay.rand]);


}).add;

)


(
// tun-ing
~train_gain = 0.05;
~tdur = 1;
~tlen = 0.4;
~tfreq = 50.0;
~min_pitch = 600.0;
~max_pitch = 4000.0;
~tjitter = 0.4;
)



(
// MIDI Cues

MIDIIn.connectAll;

~cue1 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 1 ".post;
		args[1].postln;
		(
			// launch freely
			~v1.set(\bf, [exprand(1, 10), exprand(10, 20)] );
			~v1.set(\freq, [exprand(100, 200), exprand(200, 250)]);
			~v1.set(\harmspeed, [7, 9]);
			~v1.set(\gain, [0.2, 0.5]);
			~v1.set(\vmix, 0.7);
			~v1.set(\vroom, 1);
			~v1.set(\vdamp, 0.5);
			~v1.set(\impspeed1, 9);
			~v1.set(\impspeed2, 7);
			~v1.set(\length1, 0.5);
			~v1.set(\length2, 0.1);
		)
	})
}, 81);

~cue2 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 2 ".post;
		args[1].postln;
		(
			// blob tuning
			// push max blobs up (1024 MAX -- keep the synth count reasonable: 400-500)
			~train_gain = 0.05;
			~tdur = 0.3;
			~tlen = 0.3;
			~tfreq = 1.0;
			~min_pitch = 60.0;
			~max_pitch = 400.0;
			~tjitter = 1.0;
		)
	})
}, 84);

~cue3 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 3 ".post;
		args[1].postln;
		(
			~v1.set(\bf, [400, 800] );
			~v1.set(\freq, [809, 910]);
			~v1.set(\harmspeed, [1, 2]);
			~v1.set(\gain, [0.3, 0.2]);
			~v1.set(\vmix, 0);
			~v1.set(\impspeed1, 10);
			~v1.set(\impspeed2, 10.5);
			~v1.set(\length1, 2);
			~v1.set(\length2, 2);
		)
	})
}, 87);

~cue4 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 4 ".post;
		args[1].postln;
		// launch freely...
		~v2.set(\impspeed1, rrand(1, 15));
		~v2.set(\impspeed2, rrand(1, 15));
		~v1.set(\impspeed1, rrand(0.5, 4));
		~v1.set(\impspeed2, rrand(0.5, 4));

	})
}, 90);

~cue5 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 5 ".post;
		args[1].postln;
		(
			// blob tuning
			// push max blobs up (1024 MAX -- keep the synth count reasonable: 400-500)
			~train_gain = 0.3;
			~tdur = 1;
			~tlen = 0.2;
			~tfreq = 10.0;
			~min_pitch = 10000.0;
			~max_pitch = 16000.0;
			~tjitter = 0.2;
		)

	})
}, 93);

~cue6 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 6 ".post;
		args[1].postln;
		(
			~v2.set(\bf, [400, 1600] );
			~v2.set(\freq, [200, 8019]);
			~v2.set(\harmspeed, [70, 100]);
			~v2.set(\gain, [0.6, 0.6]);
			~v2.set(\vmix, 1);
			~v2.set(\vroom, 0.3);
			~v2.set(\vdamp, 0);
			~v2.set(\impspeed1, 30);
			~v2.set(\impspeed2, 5);
			~v2.set(\length1, 0.1);
			~v2.set(\length2, 0.5);
		)

	})
}, 120);

~cue7 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 7 ".post;
		args[1].postln;

		// launch freely...
		~v2.set(\impspeed1, rrand(1, 15));
		~v2.set(\impspeed2, rrand(1, 15));
		~v1.set(\impspeed1, rrand(0.5, 4));
		~v1.set(\impspeed2, rrand(0.5, 4));

	})
}, 98);

~cue8 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 8 ".post;
		args[1].postln;
		// launch freely

		(
			~v1.set(\bf, [40, 80] );
			~v1.set(\freq, [809, 910]);
			~v1.set(\harmspeed, [21, 20]);
			~v1.set(\gain, [0.3, 0.2]);
			~v1.set(\vmix, 0.5);
			~v1.set(\impspeed1, 50);
			~v1.set(\impspeed2, 50.1);
			~v1.set(\length1, 0.3);
			~v1.set(\length2, 0.1);
		)
	})
}, 65);

~cue9 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 8 ".post;
		args[1].postln;

		~v2.set(\bf, [900, 209] );
		~v2.set(\freq, [40, 41]);
		~v2.set(\harmspeed, [2, 1]);
		~v2.set(\gain, [0.6, 0.6]);
		~v2.set(\vmix, 0.4);
		~v2.set(\vroom, 1);
		~v2.set(\vdamp, 0);
		~v2.set(\impspeed1, 1);
		~v2.set(\impspeed2, 2.1);
		~v2.set(\length1, 0.05);
		~v2.set(\length2, 0.1);
	})
}, 82);

~cue10 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 8 ".post;
		args[1].postln;
		(
			// blob tuning
			// push max blobs up (1024 MAX -- keep the synth count reasonable: 400-500)
			~train_gain = 0.1;
			~tdur = 0.5;
			~tlen = 0.01;
			~tfreq = 50.0;
			~min_pitch = 100.0;
			~max_pitch = 250.0;
			~tjitter = 0.5;
		)
	})
}, 85);

~cue11 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 8 ".post;
		args[1].postln;
		(
			~v2.set(\bf, [100, 20] );
			~v2.set(\freq, [20, 400]);
			~v2.set(\harmspeed, [80, 97]);
			~v2.set(\gain, [0.6, 0.6]);
			~v2.set(\vmix, 1);
			~v2.set(\impspeed1, 1);
			~v2.set(\impspeed2, 2.1);
			~v2.set(\length1, 0.5);
			~v2.set(\length2, 1);
			~v1.set(\fade, 0.3);
			~v2.set(\fade, 0.2);

		)
	})
}, 88);

~cue12 = MIDIFunc.cc({arg ...args;
	if( args[0] > 0, {
		"cue 8 ".post;
		args[1].postln;
		(
			~v1.set(\harmspeed, [50, 20]);
			~v1.set(\gain, [0.3, 0.2]);
			~v1.set(\vmix, 1);
			~v1.set(\vroom, 0.2);
			~v1.set(\vdamp, 0.9);
			~v1.set(\impspeed1, 10);
			~v1.set(\impspeed2, 20);
			~v1.set(\length1, 0.2);
			~v1.set(\length2, 0.2);
		)
	})
}, 91);

~v1_gain = 0.7;
~v1_set_gain = 0.0;
~v2_gain = 0.7;
~v2_set_gain = 0.0;
~v3_gain = 0.3;
~v3_set_gain = 0.0;
~voice_gain = 0.3;
~voice_set_gain = 0.0;

// voices
~v1_toggle = MIDIFunc.cc({arg ...args;
	"v1 ".post;
	args[1].post;
	~v1_set_gain = (args[0] / 127.0) * ~v1_gain;
	~v1_set_gain.postln;
	~v1.set(\fade, ~v1_set_gain);
}, 83);


~v2_toggle = MIDIFunc.cc({arg ...args;
	"v2 ".post;
	args[1].post;
	~v2_set_gain = (args[0] / 127.0) * ~v2_gain;
	~v2_set_gain.postln;
	~v2.set(\fade, ~v2_set_gain);
}, 86);

~v3_toggle = MIDIFunc.cc({arg ...args;
	"v3 ".post;
	args[1].post;
	~v3_set_gain = (args[0] / 127.0) * ~v3_gain;
	~v3_set_gain.postln;
	~v3.set(\fade, ~v3_set_gain);
}, 89);

~voice_toggle = MIDIFunc.cc({arg ...args;
	"v1 ".post;
	args[1].post;
	~voice_set_gain = (args[0] / 127.0) * ~voice_gain;
	~voice_set_gain.postln;
	~voice.set(\fade, ~voice_set_gain);
}, 92);

)


// gains
~v1_gain = 0.7;
~v2_gain = 0.7;
~v3_gain = 0.3;
~voice_gain = 0.3;


/*
BEGIN SHOW
TAKE YOUR TIME... :)
*/


// make sure outis is dark! (G)
// fade in v1

// outis on, send...

// fade in voice
//...
// fade out voice

// cue 1

// wait a sec and fade out v1

// wait for fade

// cue 2


// fade in v1, v2, v3
~v1_gain = 0.7;
~v2_gain = 0.7;
~v3_gain = 0.1;

// STOP OUTIS SEND

// cue 3

~voice_gain = 0.3;
// this should be harder to hear...

// cue 4

// OUTIS SEND

// fade out v1, v2

// pause...
// fade out v3

// fade out voice

// cue 5

// fade in v3
~v3_gain = 0.3;

// cue 6
// fade in v1
~v1_gain = 0.7;

// cue 7
// fade in v1
~v1_gain = 0.3;

// fade out v2

// fade in v3, low
~v3_gain = 0.1;

// cue 8

// fade out v3

// ******
// Glitch out OUTIS!
// ******

// wait a sec...  then

// fade out v1

// fade in v2
~v2_gain = 0.7;
// fade out v3
~v3_gain = 0;

// wait....

// fade in v3 ver low
~v3_gain = 0.1;

// cue 9

// cue 10

// ******
// Glitch out OUTIS!
// ******

// Outis --> STOP

// cue 11

// Outis --> SEND

// cue 12

// play a while, then...

// fade out v1

// wait until complete

// fade out v2

// wait until complete

// Glitch/Blur everything out crazily
// go all b&w cow shapes

// fade out v3

// fade out voice

// wait until complete
// fade out mixer...
// press 'g' key on Outis.

// END SHOW //


(
// kill everything

~v1.free;
~v2.free;
~v3.free;
~voice.free;
~blob_responder.remove;

~cue1.free;
~cue2.free;
~cue3.free;
~cue4.free;
~cue5.free;
~cue6.free;
~cue7.free;
~cue8.free;
~cue9.free;
~cue10.free;
~cue11.free;
~cue12.free;


~v1_toggle.free;
~v2_toggle.free;
~v3_toggle.free;
~voice_toggle.free;

)