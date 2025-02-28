#ifndef APP_HPP
#define APP_HPP

class App {
public:
	App();
	~App();

	void run();

private:
  	void init();
	void update();
	void render();
	void cleanup();

};

#endif //APP_HPP
