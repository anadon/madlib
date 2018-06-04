#NOTE: This does not automatically rebuild changed subdirecories, and so each
#change requires a make-clean, and make.
#TODO: make this rebuild updated subdirectories automatically


#This must be the base name of the sub directory appended with a ".o".
SPEC_OBJECTS=geo-soft.o


all: $(SPEC_OBJECTS)

$(SPEC_OBJECTS):
	$(eval SUBDIR := $(basename $@))
	$(MAKE) -C $(SUBDIR)
	ld -r $(SUBDIR)/*.o -o $@


clean:
	rm -f $(SPEC_OBJECTS)
	for obj in $(basename $(SPEC_OBJECTS) ) ; do \
		$(MAKE) -C $$obj clean; \
	done
