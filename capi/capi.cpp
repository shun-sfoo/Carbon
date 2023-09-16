#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QWidget>
#include <cstddef>

enum class MouseStatus : uint8_t {
  Explore = 0,
  Capturing,
  Captured,
};

class CapiWidget : public QWidget {
public:
  explicit CapiWidget(QImage *img) : screenImage(img) {
    setMouseTracking(true);
  }
  // TODO: copy assigned constroctor, copy assigned operator

  ~CapiWidget() { delete screenImage; }

public:
  void paintEvent(QPaintEvent *event) override {
    QPainter painter(this);
    painter.drawImage(QRect(QPoint(0, 0), this->size()), *this->screenImage);
    switch (m_status) {
    case MouseStatus::Explore: break;
    case MouseStatus::Capturing: {
      painter.setPen(QPen(Qt::red));
      int w = abs(startX - currX);
      int h = abs(startY - currY);
      int left = startX < currY ? startX : currX;
      int top = startY < currY ? startY : currY;
      painter.drawRect(left, top, w, h);
      break;
    }
    case MouseStatus::Captured: {
      painter.setPen(QPen(Qt::blue));
      painter.drawRect(capturedRect);
      break;
    }
    }
  }
  void mousePressEvent(QMouseEvent *event) override {
    // TODO: double click may bring error
    switch (m_status) {
    case MouseStatus::Explore: {
      m_status = MouseStatus::Capturing;
      currX = event->pos().x();
      currY = event->pos().y();
      // TODO: notice last position
      startX = currX;
      startY = currY;
      qDebug("mousePressEvent: x = %d, y = %d", currX, currY);
      break;
    }
    default: break;
    }
    this->update();
  }
  void mouseReleaseEvent(QMouseEvent *event) override {
    switch (m_status) {
    case MouseStatus::Capturing: {
      m_status = MouseStatus::Captured;
      int w = abs(currX - startX);
      int h = abs(currY - startY);
      int left = startX < currX ? startX : currX;
      int top = startY < currY ? startY : currY;
      capturedRect = QRect(left, top, w, h);
      break;
    }
    default: break;
    }
    this->update();
  }
  void mouseMoveEvent(QMouseEvent *event) override {
    switch (m_status) {
    case MouseStatus::Capturing: {
      currX = event->pos().x();
      currY = event->pos().y();
      break;
    }
    default: break;
    }
    this->update();
  }
  void keyPressEvent(QKeyEvent *event) override {
    if (event->key() == Qt::Key_Return && m_status == MouseStatus::Captured) {
      // 1. 获取捕获的图像区域
      auto picRealSize = screenImage->size();
      auto winSize = this->size();
      // 比例计算
      int realRectX = capturedRect.x() * picRealSize.width() / winSize.width();
      int realRectY =
          capturedRect.y() * picRealSize.height() / winSize.height();
      int realRectW =
          capturedRect.width() * picRealSize.width() / winSize.width();
      int realRectH =
          capturedRect.height() * picRealSize.height() / winSize.height();
      // 得到实际Rect
      QRect imgRect(realRectX, realRectY, realRectW, realRectH);
      // 2. 从保存的屏幕图像中获取指定区域的图像数据
      auto copiedImg = this->screenImage->copy(imgRect);
      // 3. 将图像数据写入到操作系统粘贴板
      QClipboard *clipboard = QGuiApplication::clipboard();
      clipboard->setImage(copiedImg);
      m_status = MouseStatus::Explore;
      return;
    }

    if (event->key() == Qt::Key_Escape) {
      m_status = MouseStatus::Explore;
    }
    this->update();
  }

private:
  int startX = 0, startY = 0;
  int currX = 0, currY = 0;

private:
  MouseStatus m_status = MouseStatus::Explore;

private:
  QRect capturedRect;
  QImage *screenImage;
};

int main(int argc, char *argv[]) {
  QApplication a(argc, argv); // must be have
  QScreen *screen = QApplication::screenAt(QCursor().pos());
  QImage img = screen->grabWindow(0).toImage();
  CapiWidget w(&img);
  w.resize(800, 450);
  w.show();
  return QApplication::exec();
}
