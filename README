# mousetrap(1) — filter for mouse events

`mousetrap(1)` is designed to be interposed between a `nusb/kb(4)` mouse
driver and the mouse event destination `/dev/mousein`, which provides an
opportunity to intercept the "raw" mouse events and transform them
before delivery to the rest of the system.

Example transformations include remapping and hooking up arbitrary
actions to buttons, which is mainly useful for devices with more buttons
than the *Plan 9* recognizes. Yet another example might be filtering the
pointer movement of overly sensitive devices.

If you plan on using this you are expected to modify the source code to
suit your devices and needs.  Some example mappings are provided to
help with this.

## Usage

`mousetrap(1)` can be installed into place, prior to running `nusb/kb(4)`
using the `pipefile(1)` utility as follows:

	pipefile -r 'mousetrap -m elecom' /dev/mousein
	nusb/kb 9

## TODO

In the (near?) future `filterfs(4)` of some designation should be
replace `pipefile(1)` as a more Plan9esque filtering solution.  Sigrid
of 9front works on this: https://git.sr.ht/~ft/filterfs

Figure out a satisfactory way to integrate this kind of device filtering
with the *9front* boot mechanism.  Currently the `/sys/src/9/boot/nusbrc`
has to be modified as per above and then a kernel has to be recompiled
and installed for it to take effect.  We agree this kinds of sucks.

A temporary workaround, if you can get away with it, is `kill(1)`ing
the `nusb/kb` that got assigned to your device at boot and restarting
it manually. How to do this one is left as an exercise for the brave
reader. HINT: `/dev/usbevent`.
