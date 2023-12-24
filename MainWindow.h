
#ifndef CARTES_H
#define CARTES_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QResizeEvent>
#include <QVBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class cartes; }
QT_END_NAMESPACE

class CustomLabel : public QLabel {
    Q_OBJECT


private:
    QString imagePath;
    QVector<QLabel*> computerCards; // Cartes de l'ordinateur
    QVector<QLabel*> playerCards;   // Cartes du joueur

public:
    explicit CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {}
    QString getImagePath() const;
    void setImagePath(const QString &path);
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }
};

class cartes : public QMainWindow
{
    Q_OBJECT

public:
    cartes(QWidget *parent = nullptr);
    ~cartes();

    void onCardSelected(int cardValue);
    void processCardValue(const QString& cardValue);
    void moveCardsToCenter(QLabel* card1, QLabel* card2);
    void compareAndMoveCardsForPlayer(QVector<QLabel*> &cards);
    void playTurn();
    void checkEndGame();
    void distributeInitialCards();
    int countRemainingCards(bool isComputer) ;
    void simulateComputerTurn();
    void addComputerCard();
    void afficherResultat();

signals:
    void playerTurnSignal();
    void computerTurnSignal();



private slots:

    // void passImages(int imageId);
    void clearPreviousCards();
    void addCards();
    void positionCartes();
    QString extractCardValue(const QString &imageName);
    void processCardTake(const QString &imagePath);
    void startGame();
    void playerPlaceCard();
    void computerPlaceCard();
    void resetGame(); // Déclaration de la fonction resetGame
    void exitGame();
    void moveCardToCenter(QLabel* card);
    void selectPlayerCard(QLabel* selectedCard);


private:
    Ui::cartes *ui;
     QList<QLabel*> prises;
    QList<QLabel*> computerCards;// Cartes de l'ordinateur
    QList<QLabel*> playerCards;   // Cartes du joueur
    QList<QString> cardImages;
    QString imagePath;

    void setupUI();
    bool playerTurn;
    bool gameStarted; // Indicateur pour suivre le démarrage du jeu
    QString computerCardBackImagePath = ":/k/imagrBack.png";
    //QString cardFaceImagePath = ":/k/cardFaceImage.png"; // Ajoutez cette ligne pour déclarer la variable pour l'image de face des cartes


};
#endif // CARTES_H


/*

#ifndef CARTES_H
#define CARTES_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QResizeEvent>
#include <QVBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class cartes; }
QT_END_NAMESPACE

class cartes : public QMainWindow
{
    Q_OBJECT

public:
    cartes(QWidget *parent = nullptr);

    ~cartes();
     void moveCardsToCenter(QLabel* card1, QLabel* card2);

private slots:

    void clearPreviousCards();
    void addCards();
    void centerCards();
    QString extractCardValue(const QString &imageName);
    void processCardTake(const QString &imagePath);
    void startGame();
    void resetGame(); // Déclaration de la fonction resetGame
    void exitGame();
    void checkEndGame();



  //  void moveMatchedCardsToCenter(QVector<QLabel*>& cards);
 //   void compareAndMoveCards(); // Fonction pour comparer les cartes et les déplacer



private:
    Ui::cartes *ui;
    QVector<QLabel*> prises;
    bool gameStarted; // Indicateur pour suivre le démarrage du jeu
    QString computerCardBackImagePath = ":/k/imagrBack.png";
    QString cardFaceImagePath = ":/k/cardFaceImage.png"; // Ajoutez cette ligne pour déclarer la variable pour l'image de face des cartes

};
#endif // CARTES_H


*/


/*

#ifndef CARTES_H
#define CARTES_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QResizeEvent>
#include <QVBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class cartes; }
QT_END_NAMESPACE

class cartes : public QMainWindow
{
    Q_OBJECT

public:
    cartes(QWidget *parent = nullptr);
    ~cartes();


private slots:

    void clearPreviousCards();
    void addCards();
    void distributeCards();
    QString extractCardValue(const QString &imageName);
    void processCardTake(const QString &imagePath);
    void startGame();
    void compareAndMoveCards(); // Fonction pour comparer les cartes et les déplacer


private:
    Ui::cartes *ui;
    QVector<QLabel*> prises;
    bool gameStarted; // Indicateur pour suivre le démarrage du jeu
    QString computerCardBackImagePath = ":/k/imagrBack.png";
 QString cardFaceImagePath = ":/k/cardFaceImage.png"; // Ajoutez cette ligne pour déclarer la variable pour l'image de face des cartes
};
#endif // CARTES_H
*/
