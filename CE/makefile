CPPFLAGES = -O2 -Wall


all: ce

ce: main.o cache.o lru_core.o lru_policy.o
	g++ $(CPPFLAGES) -o $@ $^

lru_core.o: lru_core.cpp lru_core.h
	g++ $(CPPFLAGES) -c $<

lru_policy.o: lru_policy.cpp lru_policy.h lru_core.h policy.h
	g++ $(CPPFLAGES) -c $<

cache.o: cache.cpp cache.h
	g++ $(CPPFLAGES) -c $<

main.o: main.cpp cache.h lru_policy.h policy.h
	g++ $(CPPFLAGES) -c $<


.PHONY: clean
clean:
	rm -rf *.o
