#ifndef TRACER_H
#define TRACER_H

class Tracer {
 public:
  /**
   * コンストラクタ
   */
  Tracer();

  /**
   * @brief 走行する抽象メソッド
   */
  virtual void run() = 0;

};

#endif