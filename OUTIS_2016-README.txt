
Outis.app - Michael Clemow, 2009

KEY COMMANDS

's' - start/stop sending OSC messages to port 12345
'a' - send test OSC message /test f (should send a 500.0)

'-' - decrease threshold value
'=' - increase threshold value
'[' - decrease max blobs
']' - increase max blobs
'{' - decrease blur amount
'}' - increase blur amount
';' - decrease minimum blob size
''' - increase minimun blob size
'<' - decrease posterization level
'>' - increase posterization level

't' - toggle fullscreen
'b' - toggle blur
'p' - toggle posterization
'i' - toggle invert
'o' - toggle line offset (glitches out the video)
'9' - decrease offset value
'0' - increase offset value

'g' - toggle GUI
'h' - toggle HUD
'w' - toggle threshold BW image
'e' - toggle full BW image
'y' - toggle info
'j' - toggle patch
'c' - toggle constrast stretching

',' - decrease playback speed
'.' - increase playback speed
'z' - set loop in
'x' - set loop out
'd' - drop loop (normal playback)
'n' - set playback speed to normal (1.0)


OSC MESSAGE FORMAT

/blob iffffffffiiiii

i - frame number (0+)
i - number of blobs in frame (0+)
f - area of the blob (0+)
f - length of blob surface outline (0+)
f - bounding rect X (0+)
f - bounding rect Y (0+)
f - bounding rect width (0+)
f - bounding rect height (0+)
f - centroid X (0+)
f - centroid Y (0+)
i - red value at centroid (0-255)
i - green value at centroid (0-255)
i - blue value at centroid (0-255)
i - hole (0 = no hole in blob, 1 = hole in blob) i.e. blob has a blob inside it
i - number of points in the blob contour (0+)


