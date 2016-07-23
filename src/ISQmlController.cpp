#include <ISQmlController.h>

ISQmlController::ISQmlController(QObject *parent) : QObject(parent)
{

}

void ISQmlController::sampleSlot()
{
    emit sample();
}
