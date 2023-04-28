#include <sl/Camera.hpp>

// Sample includes
static bool exit_app = false;

// Handle the CTRL-C keyboard signal
#ifdef _WIN32
#include <Windows.h>

void CtrlHandler(DWORD fdwCtrlType) {
    exit_app = (fdwCtrlType == CTRL_C_EVENT);
}
#else
#include <signal.h>
void nix_exit_handler(int s) {
    exit_app = true;
}
#endif

// Set the function to handle the CTRL-C
void SetCtrlHandler() {
#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);
#else // unix
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = nix_exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
#endif
}

// Using namespace
using namespace sl;

void runner(sl::Camera& zed, bool& exit, int id) {
	auto cam_infos = zed.getCameraInformation().camera_configuration;
	int fps = cam_infos.fps;
	std::cout << "ZED[" << id << "]" << " Start recording "<< cam_infos.resolution.width<<"@"<< fps <<"\n";
	
    int frames_recorded = 0;
	int nb_grabbed = 0;

	auto start = std::chrono::high_resolution_clock::now();
	sl::RecordingStatus state;
    while (!exit) {
        if (zed.grab() == ERROR_CODE::SUCCESS) {
            // Each new frame is added to the SVO file
			if (1) {
				state = zed.getRecordingStatus();
				if (state.status) frames_recorded++;
				nb_grabbed++;
				if ((nb_grabbed % fps) == 0)
					std::cout << "Z" << id << ": " << nb_grabbed << "\n";
			}
		}
    }
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
	float nb_frame_theoric = (duration / 1000.f) * fps;

	std::cout << "Recording status ["<< id<<"]: theoric " << nb_frame_theoric << ", grabed " << nb_grabbed << ", saved " << frames_recorded << " frames, avg compression time "<< state.average_compression_time<<"ms\n";
}

int main(int argc, char **argv) {
    // Set configuration parameters for the ZED
    InitParameters initParameters;	
	std::cout << "ARG 2: RESOLUTION ID: 0:2K / 1:FHD / 3:HD / 3:VGA" << std::endl;
	if (argc >= 2) 
		initParameters.camera_resolution = static_cast<RESOLUTION>(atoi(argv[1]));	
	std::cout << "Open Camera in: " << initParameters.camera_resolution << std::endl;
    initParameters.depth_mode = DEPTH_MODE::NONE;

	if(argc == 3)
		initParameters.camera_fps = atoi(argv[2]);

	const size_t NB_ZED = Camera::getDeviceList().size();

    // Create a ZED camera
    std::vector<Camera> zeds(NB_ZED);
    std::vector<std::thread> pool(NB_ZED);
    std::cout << "Try to open " << NB_ZED << " ZEDs\n";

	RecordingParameters rec_p;
	rec_p.compression_mode = SVO_COMPRESSION_MODE::H264;

    for (size_t z = 0; z < NB_ZED; z++) {
		initParameters.input.setFromCameraID(z);

        // Open the camera
        ERROR_CODE err = zeds[z].open(initParameters);
        if (err != ERROR_CODE::SUCCESS)
            std::cout <<"ZED ["<<z<<"] can not be opened, "<< err<< std::endl;
		else {
			auto SN = zeds[z].getCameraInformation().serial_number;
			std::string svo_name("SVO_SN" + std::to_string(SN) + ".svo");
			// Enable recording with the filename specified in argument
			rec_p.video_filename = String(svo_name.c_str());            
            auto err = zeds[z].enableRecording(rec_p);
            if (err != ERROR_CODE::SUCCESS)
                std::cout << "ZED [" << z << "] can not record, " << err << std::endl;			
		}
    }

    // Start recording SVO, stop with Ctrl-C command
    std::cout << "SVO is Recording, use Ctrl-C to stop." << std::endl;
    SetCtrlHandler();

	for (int z = 0; z < NB_ZED; z++) 
		if(zeds[z].isOpened())
			pool[z] = std::thread(runner, std::ref(zeds[z]), std::ref(exit_app), z);

    while (!exit_app)
        sl::sleep_us(50);
   
    for (int z = 0; z < NB_ZED; z++)
        if (zeds[z].isOpened())
            pool[z].join();

	for (int z = 0; z < NB_ZED; z++) 
		if (zeds[z].isOpened()) 
			zeds[z].disableRecording();

	for (int z = 0; z < NB_ZED; z++)
		zeds[z].close();

    return 0;
}
