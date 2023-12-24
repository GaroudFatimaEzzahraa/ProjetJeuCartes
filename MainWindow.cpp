#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QList>
#include <QDebug>
#include <QResizeEvent>
#include <QMap>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>

cartes::cartes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartes)
{
    ui->setupUi(this);

    // Initialisation des membres
    playerTurn = true; // Le joueur commence en premier

    prises = QList<QLabel*>();
    gameStarted = false; // Initialisation du jeu comme non démarré

    // Créer le fond d'écran avec un bouton de démarrage
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QPixmap backgroundImage(":/k/logoSota.png");
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    centralWidget->setPalette(palette);

    QPushButton *startButton = new QPushButton("Commencer le jeu", centralWidget);
    startButton->setObjectName("startButton"); // Pour la définition du nom du bouton

    QPushButton *resetButton = new QPushButton("Réinitialiser le jeu", centralWidget);
                               resetButton->setObjectName("resetButton"); // Nommez le bouton

    // Style du bouton
    resetButton->setStyleSheet("background-color:  #F5F5DC ; color: #A52A2A; padding: 8px; border-radius: 10px;");
    resetButton->hide(); // Masquer initialement le bouton de réinitialisation

    layout->addWidget(resetButton, 0, Qt::AlignCenter);

    // Connecter le signal clicked() du bouton à la fonction pour réinitialiser le jeu
    connect(resetButton, &QPushButton::clicked, this, &cartes::resetGame);

    // j'ai crée la forme du bouton ici
    startButton->setStyleSheet("background-color:  #F5F5DC ; color: #A52A2A; padding: 8px; border-radius: 10px;");
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    connect(startButton, &QPushButton::clicked, this, &cartes::startGame);

    setCentralWidget(centralWidget);

    QPushButton *quitButton = new QPushButton("Quitter le jeu", centralWidget);
    quitButton->setObjectName("quitButton"); // Nommez le bouton

    // Style du bouton
    quitButton->setStyleSheet("background-color:  #F5F5DC ; color: #A52A2A; padding: 8px; border-radius: 10px;");

    layout->addWidget(quitButton, 0, Qt::AlignCenter);

    // Connecter le signal clicked() du bouton à la fonction pour quitter le jeu
    connect(quitButton, &QPushButton::clicked, this, &cartes::exitGame);

}



void cartes::startGame() {
    if (!gameStarted) {
        gameStarted = true; // Marquer le jeu comme démarré
        clearPreviousCards(); // Méthode pour effacer les cartes existantes
        addCards();


        distributeInitialCards();


        // Masquer le bouton une fois le jeu commencé
        QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
        if (startButton) {
            startButton->hide();
        }

        // Rendre visible le bouton de réinitialisation une fois le jeu démarré
        QPushButton *resetButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("resetButton"));
        if (resetButton) {
            resetButton->show();
        }

        // Une fois le jeu commencé, masquez le fond d'écran
        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }

    if (gameStarted) {

        // Connecter les signaux des cartes du joueur au slot correspondant
        QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
        for (CustomLabel *customCard : customCards) {
            connect(customCard, &CustomLabel::clicked, this, &cartes::playerPlaceCard);
        }
    }

}



//Pour quitter le jeu
void cartes::exitGame() {
    qApp->exit(); // Quitter l'application lorsque le bouton est cliqué
}






//Fonction qui réinitialise le jeu
void cartes::resetGame() {
    gameStarted = false; // Marquer le jeu comme non démarré

    // Masquer le bouton de réinitialisation
    QPushButton *resetButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("resetButton"));
    if (resetButton) {
        resetButton->show();
    }

    // Afficher à nouveau le bouton "Commencer le jeu"

    QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
    if (startButton) {
        startButton->hide();
    }

    clearPreviousCards(); // Effacer les cartes existantes
    addCards(); // Ajouter de nouvelles cartes
    std::random_shuffle(prises.begin(), prises.end()); // Mélanger les cartes
    positionCartes(); // Distribuer les cartes

    gameStarted = true;
    if (gameStarted) {

        // Connecter les signaux des cartes du joueur au slot correspondant
        QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
        for (CustomLabel *customCard : customCards) {
            connect(customCard, &CustomLabel::clicked, this, &cartes::playerPlaceCard);
        }
    }
}









void CustomLabel::setImagePath(const QString &path) {
    imagePath = path;
}

QString CustomLabel::getImagePath() const {
    return imagePath;
}









//Fonction qui ajouter des cartes et initialiser la liste des images de cartes
void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" <<":/k/D10.jpg"<< ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S10.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z10.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

    for (const QString &imagePath : cardImages) {
        processCardTake(imagePath);
    }
}









//fonction de distribution initial des cartes
void cartes::distributeInitialCards() {

    // Distribution initiale des cartes entre le joueur et l'ordinateur
    for (int i = 0; i < prises.size(); ++i) {
        QLabel* card = prises.at(i);
        if (i % 2 == 0) {
            computerCards.append(card);
            card->setProperty("isComputerCard", true);
        } else {
            playerCards.append(card);
            card->setProperty("isComputerCard", false);
        }
    }
    positionCartes(); // Appel pour positionner les cartes sur l'interface

}








//Fonction qui ajouter une nouvelle carte dans le jeu

void cartes::processCardTake(const QString& imagePath)
{
    CustomLabel* newCard = new CustomLabel(this);

    QPixmap cardImage(imagePath);

    if (cardImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de la carte.";
        return;
    }

    cardImage = cardImage.scaled(80, 100, Qt::KeepAspectRatio);

    newCard->setPixmap(cardImage);

    int cardHeight = 100;

    //  je positionnée  les nouvelles cartes à des coordonnées aléatoires
    int newX = QRandomGenerator::global()->bounded(width() - 80); // Nouvelles coordonnées X
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y
    newCard->setGeometry(newX, newY, 80, 100);

    newCard->show();

    prises.append(newCard);
}









//Positionnement des cartes sur l'interface de jeu

void cartes::positionCartes() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas


    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffsetTop = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en haut
    int horizontalOffsetBottom = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en bas

    bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i); //récupère la carte à la position 'i'

        QPropertyAnimation *moveAnimation = new QPropertyAnimation(card, "geometry");
        moveAnimation->setDuration(1000);

        int newX, newY;

        if (i % 2 == 0) { // Ligne du haut
            newX = horizontalOffsetTop;
            newY = topY;
            horizontalOffsetTop += cardWidth + spacing; // Augmenter l'offset horizontal pour la prochaine carte en haut

        } else { // Ligne du bas
            newX = horizontalOffsetBottom;
            newY = bottomY;
            horizontalOffsetBottom += cardWidth + spacing; // Augmenter l'offset horizontal pour la prochaine carte en bas
        }

        moveAnimation->setStartValue(card->geometry());//Configure les valeurs de départ pour l'animation
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));//Configure les valeurs d'arrivée pour l'animation
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);//Définit une courbe d'animation

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
            QPixmap cardBackImage(computerCardBackImagePath);
            card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

   // compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}










//Fonction qui effacer les cartes précédentes
void cartes::clearPreviousCards() {
    for (QLabel* card : qAsConst(prises)) {
        delete card;
    }
    prises.clear();
}







//Fonction qui affiche la fin de jeu
void cartes::checkEndGame() {
    int computerCardCount = computerCards.size();
    int playerCardCount = playerCards.size();

    if (computerCardCount == 0) {
        qDebug() << "Ooo Vous avez perdu!";
        QMessageBox::information(this, "Fin du jeu", "Ooo Vous avez perdu!");
        gameStarted = false;
    } else if (playerCardCount == 0) {
        qDebug() << "Yeeeh Vous avez gagné!";
            QMessageBox::information(this, "Fin du jeu", "Yeeeh Vous avez gagné!");
            gameStarted = false;
    }
}

int cartes::countRemainingCards(bool isComputer) {
    int count = 0;
    for (QLabel *card : prises) {
        bool cardOwner = card->property("isComputerCard").toBool();
        if (cardOwner == isComputer) {
            count++;
        }
    }
    return count;
}




void cartes::playerPlaceCard() {
    if (playerTurn) {
        CustomLabel *selectedCard = qobject_cast<CustomLabel *>(sender());
        if (selectedCard) {
            selectPlayerCard(selectedCard); // Appel de selectPlayerCard() avec la carte sélectionnée par le joueur


            QString currentCardImagePath = selectedCard->getImagePath(); // Chemin de l'image de la carte actuelle
            int currentIndex = cardImages.indexOf(currentCardImagePath);
            if (currentIndex != -1 && currentIndex + 1 < cardImages.size()) {
                QString nextCardImagePath = cardImages[currentIndex + 1];
                // Utilisez nextCardImagePath pour afficher la carte suivante
                QLabel* nextCardLabel = new QLabel;
                nextCardLabel->setPixmap(QPixmap(nextCardImagePath));
                nextCardLabel->show();
            }

            playerTurn = !playerTurn;
            checkEndGame(); // Appel à la fonction pour vérifier si le joueur a gagné après avoir joué sa carte

            // Après que le joueur a joué sa carte, l'ordinateur joue la sienne
            computerPlaceCard();

            // Vérifiez si la carte du joueur correspond à une carte de l'ordinateur
            QString cardValue = extractCardValue(selectedCard->property("cardValue").toString());

        }
    }
}






void cartes::computerPlaceCard() {
    if (!playerTurn) {
        CustomLabel *selectedCardcomputer = qobject_cast<CustomLabel *>(sender());
        QString currentCardImagePath = selectedCardcomputer->getImagePath(); // Chemin de l'image de la carte actuelle
        int currentIndex = cardImages.indexOf(currentCardImagePath);
        if (currentIndex != -1 && currentIndex + 1 < cardImages.size()) {
            QString nextCardImagePath = cardImages[currentIndex + 1];
            QLabel* nextCardLabel = new QLabel;
            nextCardLabel->setPixmap(QPixmap(nextCardImagePath));
            nextCardLabel->show();
        }

        QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
        moveCardToCenter(computerSelectedCard);
        playerTurn = !playerTurn;
        //  compareAndMoveCards();
        checkEndGame(); // Appel à la fonction pour vérifier si l'ordinateur a gagné après avoir joué sa carte
    }
}





//fonction qui anime le déplacement d'une carte vers le centre de la fenêtre
void cartes::moveCardToCenter(QLabel* card) {
    int cardWidth = 80;
    int cardHeight = 100;
    int centerOffset = (width() - cardWidth) / 2;

    // Animation pour déplacer la carte au centre
    QPropertyAnimation *moveAnimation = new QPropertyAnimation(card, "geometry");
    moveAnimation->setDuration(1000);
    moveAnimation->setStartValue(card->geometry());
    moveAnimation->setEndValue(QRect(centerOffset, height() / 2, cardWidth, cardHeight));
    moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // Démarrer l'animation
    moveAnimation->start();

    // Mettre à jour l'interface après l'animation
    QTimer::singleShot(1000, this, [=]() {
       // compareAndMoveCards();
    });
}








void cartes::selectPlayerCard(QLabel* selectedCard) {
    if (playerTurn) { // Vérifie si c'est le tour du joueur
        QString selectedCardValue = extractCardValue(selectedCard->property("cardValue").toString());

        // Comparer la carte sélectionnée par le joueur avec les cartes de l'ordinateur
        for (QLabel* computerCard : computerCards) {
            QString computerCardValue = extractCardValue(computerCard->property("cardValue").toString());

            if (selectedCardValue == computerCardValue) {
                moveCardToCenter(computerCard); // Déplacer la carte de l'ordinateur au centre

                // Retirer les cartes des vecteurs respectifs
                playerCards.removeAll(selectedCard);
                computerCards.removeAll(computerCard);

                // Inverser le tour du joueur après que le joueur ait joué
                playerTurn = !playerTurn;
                checkEndGame(); // Vérifier la fin du jeu
                return; // Sortir de la fonction dès qu'une carte est trouvée
            }
        }
    }
}







// Méthode pour déplacer les cartes au centre de l'interface
void cartes::moveCardsToCenter(QLabel* card1, QLabel* card2) {
    int cardWidth = 80;
    int cardHeight = 100;
    int centerOffset = (width() - (2 * cardWidth)) / 2;

    // Animation pour déplacer la première carte
    QPropertyAnimation *moveAnimation1 = new QPropertyAnimation(card1, "geometry");
    moveAnimation1->setDuration(1000);
    moveAnimation1->setStartValue(card1->geometry());
    moveAnimation1->setEndValue(QRect(centerOffset, height() / 2, cardWidth, cardHeight));
    moveAnimation1->setEasingCurve(QEasingCurve::InOutQuad);

    // Animation pour déplacer la deuxième carte
    QPropertyAnimation *moveAnimation2 = new QPropertyAnimation(card2, "geometry");
    moveAnimation2->setDuration(1000);
    moveAnimation2->setStartValue(card2->geometry());
    moveAnimation2->setEndValue(QRect(centerOffset + cardWidth, height() / 2, cardWidth, cardHeight));
    moveAnimation2->setEasingCurve(QEasingCurve::InOutQuad);

    // Créer un groupe d'animations pour les exécuter simultanément
    QParallelAnimationGroup *group = new QParallelAnimationGroup();
    group->addAnimation(moveAnimation1);
    group->addAnimation(moveAnimation2);

    // Démarre l'animation
    group->start();

    // Cacher les cartes déplacées en remplaçant l'image par celle du dos de carte
    QPixmap cardBackImage(computerCardBackImagePath);
    card1->setPixmap(cardBackImage);
    card2->setPixmap(cardBackImage);

    // Mettre à jour l'interface après l'animation
    QTimer::singleShot(1000, this, [=]() {
        update();
    });
}











// Extraire la valeur de la carte à partir du nom de l'image

QString cartes::extractCardValue(const QString &imageName) {
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Parce que le nom de l'image est dans le format XY.jpg (ex: D10.jpg)(X est la valeur de la carte)
    return cardValue;
}










//Destructeur
cartes::~cartes()
{
    clearPreviousCards(); // Pour nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}



