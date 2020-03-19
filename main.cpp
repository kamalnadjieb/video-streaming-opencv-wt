#include <mutex>

#include <opencv2/opencv.hpp>

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WMemoryResource.h>
#include <Wt/WServer.h>
#include <Wt/WTimer.h>

std::vector<unsigned char> buf;
std::mutex buf_mtx;

class VideoStreamingApplication : public Wt::WApplication
{
public:
    VideoStreamingApplication(const Wt::WEnvironment& env);

private:
    Wt::WImage* image_;
    Wt::WTimer* timer_;
};

VideoStreamingApplication::VideoStreamingApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
{
    setTitle("Stream");
    image_ = root()->addWidget(std::make_unique<Wt::WImage>());
    timer_ = root()->addChild(std::make_unique<Wt::WTimer>());
    timer_->setInterval(std::chrono::milliseconds(50));
    auto reload = [this]
    {
        std::lock_guard<std::mutex> lck (buf_mtx);
        image_->setImageLink(Wt::WLink(std::make_shared<Wt::WMemoryResource>("mime/jpeg", buf)));
    };
    timer_->timeout().connect(reload);
    timer_->start();
}

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env)
{
  return std::make_unique<VideoStreamingApplication>(env);
}

int main(int argc, char** argv)
{
    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open video capture" << std::endl;
        exit(1);
    }

    Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
    server.addEntryPoint(Wt::EntryPointType::Application, createApplication);

    server.start();

    while(server.isRunning())
    {
        cv::Mat image;
        cap >> image;

        if (image.empty())
        {
            std::cerr << "ERROR! blank frame grabbed" << std::endl;
            exit(1);
        }

        std::lock_guard<std::mutex> lck (buf_mtx);
        cv::imencode(".jpeg", image, buf);
    }

    server.stop();

    cap.release();
}
