
PROJECTS:=bootloader demo_composite_hid demo_hidapi demo_gamepad demo_mouse_jiggler testing/cdc_exp testing/demo_midi testing/sandbox testing/test_ethernet

all : build

build :
	for dir in $(PROJECTS); do make -C $$dir build; done

clean : $(PROJECTS)
	for dir in $(PROJECTS); do make -C $$dir clean; done

.PHONY : $(PROJECTS)

