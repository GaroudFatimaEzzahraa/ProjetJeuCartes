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





















/*#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileInfo>
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
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
#include <iostream>

cartes::cartes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartes)
{
    ui->setupUi(this);

    // Initialisation des membres
    playerTurn = true; // Le joueur commence en premier

    prises = QVector<QLabel*>();
    gameStarted = false; // Initialisation du jeu comme non démarré

    // Création de fond d'écran avec un bouton de démarrage
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QPixmap backgroundImage(":/k/logoSota.png");
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    centralWidget->setPalette(palette);

    QPushButton *startButton = new QPushButton("Commencer le jeu", centralWidget);
    startButton->setObjectName("startButton"); // Pour la définition du nom du bouton

    setupUI();


    for (int i = 0; i < 12; ++i) {
        buttons[i] = new QPushButton(QString("Carte %1").arg(i + 1));
        connect(buttons[i], &QPushButton::clicked, this, [=]() {
            passImages(i + 1);
        });
        layout->addWidget(buttons[i]);
    }


    connect(buttonForCard1, &QPushButton::clicked, this, [=]() {
        onCardSelected(1); // Appel de la fonction onCardSelected avec la valeur de la carte 1
    });

    connect(buttonForCard2, &QPushButton::clicked, this, [=]() {
        onCardSelected(2); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard3, &QPushButton::clicked, this, [=]() {
        onCardSelected(3); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard4, &QPushButton::clicked, this, [=]() {
        onCardSelected(4); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard5, &QPushButton::clicked, this, [=]() {
        onCardSelected(5); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard6, &QPushButton::clicked, this, [=]() {
        onCardSelected(6); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard7, &QPushButton::clicked, this, [=]() {
        onCardSelected(7); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard8, &QPushButton::clicked, this, [=]() {
        onCardSelected(8); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard9, &QPushButton::clicked, this, [=]() {
        onCardSelected(9); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard10, &QPushButton::clicked, this, [=]() {
        onCardSelected(10); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard11, &QPushButton::clicked, this, [=]() {
        onCardSelected(11); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });

    connect(buttonForCard12, &QPushButton::clicked, this, [=]() {
        onCardSelected(12); // Appel de la fonction onCardSelected avec la valeur de la carte 2
    });


    resetButton = new QPushButton("Reset", this);
    quitButton = new QPushButton("Quit", this);


    // Connexion du signal clicked() du bouton de réinitialisation à la fonction resetGame
    connect(resetButton, &QPushButton::clicked, this, &cartes::resetGame);



    setCentralWidget(centralWidget);


    // Connexion du signal clicked() du bouton "Commencer le jeu" à la fonction startGame
    connect(startButton, &QPushButton::clicked, this, &cartes::startGame);

    // Connexion du signal clicked() du bouton "Quitter le jeu" à la fonction exitGame
    connect(quitButton, &QPushButton::clicked, this, &cartes::exitGame);



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
        gameStarted = true;
        clearPreviousCards();
        int imageId = imagePath.toInt();  // Conversion de la QString en int

        passImages(imageId);
        distributeInitialCards();

        QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
        if (startButton) {
            startButton->hide();
        }

        QPushButton *resetButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("resetButton"));
        if (resetButton) {
            resetButton->show();
        }

        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }

    // Connecter les signaux des cartes du joueur au slot correspondant
    QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
    for (CustomLabel *customCard : customCards) {
        connect(customCard, &CustomLabel::clicked, this, &cartes::playerPlaceCard);
    }
}













void cartes::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Création des boutons pour les cartes numérotées de 1 à 12
    for (int i = 0; i < 12; ++i) {
        buttons[i] = new QPushButton(QString("Carte %1").arg(i + 1));
        connect(buttons[i], &QPushButton::clicked, this, [=]() {
            passImages(i + 1);
        });
        layout->addWidget(buttons[i]);
    }
}











QString cartes::getImagePath(int cardValue) {
    QString imagePath;

    switch (cardValue) {
    case 1:
        imagePath = ":/k/S1.jpg";
        break;
    case 2:
        imagePath = ":/k/S2.jpg";
        break;
    case 3:
        imagePath = ":/k/S3.jpg";
        // ... ajouter toutes les images correspondant à la valeur 1
        break;
    case 4:
        imagePath = ":/k/S4.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 5:
        imagePath = ":/k/S5.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 6:
        imagePath = ":/k/S6.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 7:
        imagePath = ":/k/S7.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 8:
        imagePath = ":/k/S8.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 9:
        imagePath = ":/k/S9.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 10:
        imagePath = ":/k/S10.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 11:
        imagePath = ":/k/S11.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 12:
        imagePath = ":/k/S12.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    default:
        // Gestion du cas par défaut si nécessaire
        imagePath = ":/k/default.jpg"; // Chemin d'image par défaut
        break;
    }

    return imagePath;
}











void cartes::passImages(int imageId) {
    //  int cardIntValue = cardValue.toInt();

    qDebug() << "Image path for card " << imageId << ":";


    QString playerImagePath;
    QString computerImagePath;

    // Utilisation d'une instruction switch pour sélectionner une image basée sur la valeur de la carte
    switch (imageId) {
    case 1:
        imagePath = ":/k/S1.jpg";
        // ... ajouter toutes les images correspondant à la valeur 1
        break;
    case 2:
        imagePath = ":/k/S2.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 3:
        imagePath = ":/k/S3.jpg";
        // ... ajouter toutes les images correspondant à la valeur 1
        break;
    case 4:
        imagePath = ":/k/S4.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 5:
        imagePath = ":/k/S5.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 6:
        imagePath = ":/k/S6.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 7:
        imagePath = ":/k/S7.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 8:
        imagePath = ":/k/S8.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 9:
        imagePath = ":/k/S9.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 10:
        imagePath = ":/k/S10.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 11:
        imagePath = ":/k/S11.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 12:
        imagePath = ":/k/S12.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    default:
        // Gérer le cas par défaut si nécessaire
        break;
    }

    qDebug() << imagePath;

    int computerCardValue = imageId - 1; // Carte de l'ordinateur, par exemple, une valeur inférieure de 1 à celle du joueur


    switch (computerCardValue) {
    case 1:
        imagePath = ":/k/D1.jpg";

        break;
    case 2:
        imagePath = ":/k/D2.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 3:
        imagePath = ":/k/D3.jpg";
        // ... ajouter toutes les images correspondant à la valeur 1
        break;
    case 4:
        imagePath = ":/k/D4.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 5:
        imagePath = ":/k/D5.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 6:
        imagePath = ":/k/D6.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 7:
        imagePath = ":/k/D7.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 8:
        imagePath = ":/k/D8.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 9:
        imagePath = ":/k/D9.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 10:
        imagePath = ":/k/D10.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 11:
        imagePath = ":/k/D11.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    case 12:
        imagePath = ":/k/D12.jpg";
        // ... ajouter toutes les images correspondant à la valeur 2
        break;
    default:
        // Gérer le cas par défaut si nécessaire
        break;
    }

    qDebug() << "Player image path: " << playerImagePath;
    qDebug() << "Computer image path: " << computerImagePath;
    processCardTake(playerImagePath); // Carte du joueur
    processCardTake(computerImagePath); // Carte de l'ordinateur

}









void cartes::onCardSelected(int cardValue) {
    passImages(cardValue);
}














void cartes::processCardTake(const QString& imagePath)
{
    CustomLabel* newCard = new CustomLabel(this);

    // passImages(imagePath);

    qDebug() << "Processing card: " << imagePath;

    QPixmap cardImage(imagePath);

    if (cardImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de la carte.";
        return;
    }

    cardImage = cardImage.scaled(80, 100, Qt::KeepAspectRatio);

    newCard->setPixmap(cardImage);

    int cardHeight = 100;

    // Positionnez les nouvelles cartes de manière dynamique ici
    // Pour l'exemple, je les positionne à des coordonnées aléatoires
    int newX = QRandomGenerator::global()->bounded(width() - 80); // Nouvelles coordonnées X
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y
    newCard->setGeometry(newX, newY, 80, 100);


    std::cout << "Processing card: " << imagePath.toStdString() << std::endl;

    newCard->show();

    prises.append(newCard);
}
















int mapCardValue(const QString& value) {

    if (value == "1") {
        return 1;
    }else  if (value == "2") {
        return 2;
    }else  if (value == "3") {
        return 3;
    }else if (value == "4") {
        return 4;
    } else if (value == "5") {
        return 5;
    } else if (value == "6") {
        return 6;
    }else if (value == "7") {
        return 7;
    } else if (value == "8") {
        return 8;
    } else if (value == "9") {
        return 9;
    } else if (value == "10") {
        return 10;
    } else if (value == "11") {
        return 11;
    } else if (value == "12") {
        return 12;
    }
    else {
        // Gestion des cas par défaut
        return -1; // Valeur par défaut pour une carte non reconnue
    }
}


































// Définition de la fonction processCardValue
void cartes::processCardValue(const QString& cardValue) {
    int intValue = cardValue.toInt();
    int computerCardValue = intValue - 1;
    std::cout << "Computer's card value: " << computerCardValue << std::endl;
}







int cartes::getNumero(const QString& imagePath) {
    QString fileName = QFileInfo(imagePath).fileName();
    QString numeroStr = fileName.mid(2, fileName.indexOf('.') - 2);
    int numero = numeroStr.toInt();

    return numero;
}







void cartes::checkEndGame() {
    int computerCardCount = countRemainingCards(true); // Mettre à jour le nombre de cartes de l'ordinateur
    int playerCardCount = countRemainingCards(false); // Mettre à jour le nombre de cartes du joueur

    qDebug() << "Computer Card Count: " << computerCardCount;
    qDebug() << "Player Card Count: " << playerCardCount;

    if (computerCardCount == 0) {
        QMessageBox::information(this, "Fin du jeu", "Vous avez gagné!");
            gameStarted = false;
    } else if (playerCardCount == 0) {
        QMessageBox::information(this, "Fin du jeu", "Vous avez perdu!");
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
    //addCards(); // Ajouter de nouvelles cartes
    std::random_shuffle(prises.begin(), prises.end()); // Mélanger les cartes
    centerCards(); // Distribuer les cartes
    //  QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
    //if (startButton) {
    //   startButton->show(); // Afficher le bouton de démarrage
    //}
    gameStarted = true;
    if (gameStarted) {

        // Connecter les signaux des cartes du joueur au slot correspondant
        QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
        for (CustomLabel *customCard : customCards) {
            connect(customCard, &CustomLabel::clicked, this, &cartes::playerPlaceCard);
        }
    }
}





void cartes::exitGame() {
    qApp->exit(); // Quitter l'application lorsque le bouton est cliqué
}







void cartes::playerPlaceCard() {
    if (playerTurn) {
        CustomLabel *selectedCard = qobject_cast<CustomLabel *>(sender());
        if (selectedCard) {
            moveCardToCenter(selectedCard);
            playerTurn = !playerTurn;
            computerPlaceCard(selectedCard->property("cardValue").toString());
             moveCardToCenter(selectedCard);
        }
        else {
             QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
             for (CustomLabel *customCard : customCards) {
                 customCard->setEnabled(false); // Désactiver les cartes du joueur
             }

               QMessageBox::information(this, "Tour de l'ordinateur", "C'est le tour de l'ordinateur. Attendez son coup.");


               // Implémentez ici la logique pour que l'ordinateur joue son coup
               QTimer::singleShot(2000, this, [=]() {
                   simulateComputerTurn(); // Fonction pour simuler le tour de l'ordinateur
                   // Réactivez les interactions avec les cartes du joueur
                   for (CustomLabel *customCard : customCards) {
                       customCard->setEnabled(true); // Activer les cartes du joueur
                   }
                   // Mettez à jour l'interface utilisateur en conséquence ici
               });
        }
    }
        }

void cartes::simulateComputerTurn() {
    if (!computerCards.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(computerCards.size());
        QLabel *label = computerCards[randomIndex];
        CustomLabel *computerSelectedCard = qobject_cast<CustomLabel*>(label);

        if (computerSelectedCard) {
               QString cardValue = computerSelectedCard->property("cardValue").toString();

               moveCardToCenter(computerSelectedCard);
               computerPlaceCard(cardValue);

               checkEndGame();
        }
    }
}


void cartes::computerPlaceCard(const QString &playerCardValue) {
    if (!playerTurn) {
        int playerValue = playerCardValue.toInt();

        QLabel* computerSelectedCard = nullptr;

        for (QLabel *computerCard : computerCards) {
            QString computerCardValue = extractCardValue(computerCard->property("cardValue").toString());
            int compValue = computerCardValue.toInt();

            if (compValue > playerValue || compValue < playerValue) {
                computerSelectedCard = computerCard;
                break;
            }
        }

        if (computerSelectedCard != nullptr) {
            moveCardToCenter(computerSelectedCard);
            checkEndGame();
            playerTurn = !playerTurn;
        } else {
            playerTurn = true;
        }
    }
}








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
        compareAndMoveCards();
    });
}







void cartes::distributeInitialCards() {
     QString playerImagePath = getImagePath(1);
    QString computerImagePath = getImagePath(2);

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
    centerCards(); // Positionner les cartes sur l'interface
}












void cartes::compareAndMoveCards() {
    if (!playerTurn) {
        QLabel* computerSelectedCard = computerCards.at(QRandomGenerator::global()->bounded(computerCards.size()));
        moveCardToCenter(computerSelectedCard);
        playerTurn = !playerTurn;
    }

    checkEndGame(); // Vérifiez la fin du jeu après le déplacement des cartes
}







void cartes::compareAndMove(QVector<QLabel*>& sourceCards, QVector<QLabel*>& targetCards) {
    // Comparer les cartes du joueur actuel (sourceCards) avec les cartes de l'autre joueur (targetCards)
    for (int i = 0; i < sourceCards.size(); ++i) {
        QString cardValue = extractCardValue(sourceCards[i]->property("cardValue").toString());
        for (int j = 0; j < targetCards.size(); ++j) {
            QString targetCardValue = extractCardValue(targetCards[j]->property("cardValue").toString());
            if (cardValue == targetCardValue) {
                moveCardsToCenter(sourceCards[i], targetCards[j]);
                // Supprimer les cartes du joueur actuel et de l'autre joueur respectivement
                delete sourceCards[i];
                delete targetCards[j];
                sourceCards.remove(i);
                targetCards.remove(j);
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

    // Démarrer l'animation
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












void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear();
}













void cartes::centerCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

    //QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffsetTop = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en haut
    int horizontalOffsetBottom = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en bas

    bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

        QString imagePath = "std::random_shuffle(cardImages.begin(), cardImages.end())"; // Remplacez cette ligne avec le chemin approprié pour chaque carte

        processCardTake(imagePath);

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


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
            QPixmap cardBackImage(computerCardBackImagePath);
            card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

    compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}


















QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}







cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}
*/
/*
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
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
  //  , playerTurn(true)
   // , gameStarted(false)
{
    ui->setupUi(this);

    // Initialisation des membres
    playerTurn = true; // Le joueur commence en premier

    prises = QVector<QLabel*>();
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

        // Sélection aléatoire d'une carte pour l'ordinateur et affichage au centre
       // QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
        //moveCardToCenter(computerSelectedCard);


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

        // if (startButton) {
        //     startButton->hide();
        // }


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


void cartes::checkEndGame() {
if (gameStarted) {
    int computerCardCount = countRemainingCards(true);
    int playerCardCount = countRemainingCards(false);

    qDebug() << "Computer Card Count: " << computerCardCount;
    qDebug() << "Player Card Count: " << playerCardCount;

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
            moveCardToCenter(selectedCard);
            playerTurn = !playerTurn;
            compareAndMoveCards();
            checkEndGame(); // Appel à la fonction pour vérifier si le joueur a gagné après avoir joué sa carte
    }
}
}
void cartes::computerPlaceCard() {
if (!playerTurn) {
    QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
    moveCardToCenter(computerSelectedCard);
    playerTurn = !playerTurn;
    compareAndMoveCards();
    checkEndGame(); // Appel à la fonction pour vérifier si l'ordinateur a gagné après avoir joué sa carte
}
}

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
       compareAndMoveCards();
    });
}

void cartes::distributeInitialCards() {
    // Mettez ici le contenu de votre fonction distributeInitialCards()
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
    centerCards(); // Appel pour positionner les cartes sur l'interface

}

void cartes::compareAndMoveCards() {
    if (!playerTurn) {
        // Sélection aléatoire d'une carte pour l'ordinateur et affichage au centre
        QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
        moveCardToCenter(computerSelectedCard);

        // Inverser le tour du joueur après que l'ordinateur ait joué
        playerTurn = !playerTurn;
    }
    // Vérifier la fin du jeu
    checkEndGame();
}

void cartes::compareAndMove(QVector<QLabel*>& sourceCards, QVector<QLabel*>& targetCards) {
    // Comparer les cartes du joueur actuel (sourceCards) avec les cartes de l'autre joueur (targetCards)
    for (int i = 0; i < sourceCards.size(); ++i) {
        QString cardValue = extractCardValue(sourceCards[i]->property("cardValue").toString());
        for (int j = 0; j < targetCards.size(); ++j) {
            QString targetCardValue = extractCardValue(targetCards[j]->property("cardValue").toString());
            if (cardValue == targetCardValue) {
                moveCardsToCenter(sourceCards[i], targetCards[j]);
                // Supprimer les cartes du joueur actuel et de l'autre joueur respectivement
                delete sourceCards[i];
                delete targetCards[j];
                sourceCards.remove(i);
                targetCards.remove(j);
                return; // Sortir de la fonction dès qu'une carte est trouvée
            }
        }
    }

}
//méthode
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
    centerCards(); // Distribuer les cartes
    //  QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
    //if (startButton) {
    //   startButton->show(); // Afficher le bouton de démarrage
    //}
    gameStarted = true;
    if (gameStarted) {

        // Connecter les signaux des cartes du joueur au slot correspondant
        QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
        for (CustomLabel *customCard : customCards) {
            connect(customCard, &CustomLabel::clicked, this, &cartes::playerPlaceCard);
        }
    }
}


void cartes::exitGame() {
    qApp->exit(); // Quitter l'application lorsque le bouton est cliqué
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

    // Démarrer l'animation
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
void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear();
}

void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" << ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

    for (const QString &imagePath : cardImages) {
        processCardTake(imagePath);
    }
}


void cartes::centerCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

    //QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffsetTop = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en haut
    int horizontalOffsetBottom = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en bas

    bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        // QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

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


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
           QPixmap cardBackImage(computerCardBackImagePath);
           card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

     compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}

QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}


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

    //int cardWidth = 80;
    int cardHeight = 100;

    // Positionnez les nouvelles cartes de manière dynamique ici
    // Pour l'exemple, je les positionne à des coordonnées aléatoires
    int newX = QRandomGenerator::global()->bounded(width() - 80); // Nouvelles coordonnées X
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y
    newCard->setGeometry(newX, newY, 80, 100);


    newCard->show();



    prises.append(newCard);
}



cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}
*/

/*#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
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
    , playerTurn(true)
    , gameStarted(false)
{
    ui->setupUi(this);

    // Initialisation des membres
    playerTurn = true; // Le joueur commence en premier

    prises = QVector<QLabel*>();
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

        // Sélection aléatoire d'une carte pour l'ordinateur et affichage au centre
       // QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
        //moveCardToCenter(computerSelectedCard);


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

        // if (startButton) {
        //     startButton->hide();
        // }


        // Une fois le jeu commencé, masquez le fond d'écran
        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }
    // Connecter les signaux des cartes du joueur au slot correspondant
    QList<CustomLabel *> customCards = findChildren<CustomLabel *>();
    for (CustomLabel *customCard : customCards) {
        connect(customCard, &CustomLabel::clicked, this, &cartes::playerPlaceCard);
    }
}


void cartes::playerPlaceCard() {
    if (playerTurn) {
        CustomLabel *selectedCard = qobject_cast<CustomLabel *>(sender());
        if (selectedCard) {
            moveCardToCenter(selectedCard);

            // Inverser le tour du joueur
            playerTurn = !playerTurn;

            // Laisser l'ordinateur jouer après que le joueur ait placé sa carte
            compareAndMoveCards();
        }
    }
}
void cartes::computerPlaceCard() {
    if (!playerTurn) {
        QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
        moveCardToCenter(computerSelectedCard);

        // Inverser le tour du joueur après que l'ordinateur ait joué
        playerTurn = !playerTurn;

        // Laisser le joueur jouer après que l'ordinateur ait placé sa carte
        compareAndMoveCards();
    }
}


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
       compareAndMoveCards();
    });
}

void cartes::distributeInitialCards() {
    // Mettez ici le contenu de votre fonction distributeInitialCards()
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
    centerCards(); // Appel pour positionner les cartes sur l'interface

}

void cartes::compareAndMoveCards() {
    if (!playerTurn) {
        // Sélection aléatoire d'une carte pour l'ordinateur et affichage au centre
        QLabel* computerSelectedCard = prises.at(QRandomGenerator::global()->bounded(prises.size()));
        moveCardToCenter(computerSelectedCard);

        // Inverser le tour du joueur après que l'ordinateur ait joué
        playerTurn = !playerTurn;
    }
    // Vérifier la fin du jeu
    checkEndGame();
}

void cartes::compareAndMove(QVector<QLabel*>& sourceCards, QVector<QLabel*>& targetCards) {
    // Comparer les cartes du joueur actuel (sourceCards) avec les cartes de l'autre joueur (targetCards)
    for (int i = 0; i < sourceCards.size(); ++i) {
        QString cardValue = extractCardValue(sourceCards[i]->property("cardValue").toString());
        for (int j = 0; j < targetCards.size(); ++j) {
            QString targetCardValue = extractCardValue(targetCards[j]->property("cardValue").toString());
            if (cardValue == targetCardValue) {
                moveCardsToCenter(sourceCards[i], targetCards[j]);
                // Supprimer les cartes du joueur actuel et de l'autre joueur respectivement
                delete sourceCards[i];
                delete targetCards[j];
                sourceCards.remove(i);
                targetCards.remove(j);
                return; // Sortir de la fonction dès qu'une carte est trouvée
            }
        }
    }

}
//méthode
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
    centerCards(); // Distribuer les cartes
    //  QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
    //if (startButton) {
    //   startButton->show(); // Afficher le bouton de démarrage
    //}
}


void cartes::exitGame() {
    qApp->exit(); // Quitter l'application lorsque le bouton est cliqué
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

    // Démarrer l'animation
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
void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear();
}

void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" << ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

    for (const QString &imagePath : cardImages) {
        processCardTake(imagePath);
    }
}


void cartes::centerCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

    //QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffsetTop = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en haut
    int horizontalOffsetBottom = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en bas

    bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        // QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

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


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
            QPixmap cardBackImage(computerCardBackImagePath);
            card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

     compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}

QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}


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

    //int cardWidth = 80;
    int cardHeight = 100;

    // Positionnez les nouvelles cartes de manière dynamique ici
    // Pour l'exemple, je les positionne à des coordonnées aléatoires
    int newX = QRandomGenerator::global()->bounded(width() - 80); // Nouvelles coordonnées X
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y
    newCard->setGeometry(newX, newY, 80, 100);


    newCard->show();



    prises.append(newCard);
}

// Fonction pour vérifier si un joueur a terminé
void cartes::checkEndGame() {
    if (gameStarted) { // Vérifier si le jeu a commencé
        int computerCardCount = 0;
        int playerCardCount = 0;

        // Compter les cartes restantes pour chaque joueur
        for (QLabel *card : prises) {
            if (card->property("isComputerCard").toBool()) {
                computerCardCount++;
            } else {
                playerCardCount++;
            }
        }

        // Vérifier si un joueur n'a plus de cartes et afficher le message approprié
        if (computerCardCount == 1) {
            // Afficher un message indiquant que le joueur a gagné
            QMessageBox::information(this, "Fin du jeu", "Le joueur a gagné!");
            // Arrêter le jeu ou effectuer d'autres actions nécessaires pour la fin du jeu
        } else if (playerCardCount == 1) {
            // Afficher un message indiquant que l'ordinateur a gagné
            QMessageBox::information(this, "Fin du jeu", "L'ordinateur a gagné!");
            // Arrêter le jeu ou effectuer d'autres actions nécessaires pour la fin du jeu
        }
    }
}

cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}*/
/*
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
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

// Création d'une sous-classe personnalisée de QLabel pour gérer les événements de souris sur les cartes
class CustomLabel : public QLabel {
public:
    explicit CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", true);
            this->setProperty("clickOffset", event->pos());
        }
        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (this->property("isBeingMoved").toBool()) {
            QPoint clickOffset = this->property("clickOffset").toPoint();
            int newX = event->globalPosition().x() - clickOffset.x();
            int newY = event->globalPosition().y() - clickOffset.y();
            this->move(newX, newY);
        }
        QLabel::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", false);
        }
        QLabel::mouseReleaseEvent(event);
    }
};


cartes::cartes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartes)
{
    ui->setupUi(this);

    prises = QVector<QLabel*>();
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

        // Mélanger les cartes
        std::random_shuffle(prises.begin(), prises.end());

        // Distribuer les cartes
        centerCards();

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

       // if (startButton) {
       //     startButton->hide();
       // }


        // Une fois le jeu commencé, masquez le fond d'écran
        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }
}

//méthode
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
    centerCards(); // Distribuer les cartes
  //  QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
    //if (startButton) {
     //   startButton->show(); // Afficher le bouton de démarrage
    //}
}


void cartes::exitGame() {
    qApp->exit(); // Quitter l'application lorsque le bouton est cliqué
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

    // Démarrer l'animation
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
void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear();
}

void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" << ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

    for (const QString &imagePath : cardImages) {
        processCardTake(imagePath);
    }
}


void cartes::centerCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

    //QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffsetTop = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en haut
    int horizontalOffsetBottom = (width() - (prises.size() * (cardWidth + spacing))) / 2; // Centrer les cartes en bas

    bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        // QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

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


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
            QPixmap cardBackImage(computerCardBackImagePath);
            card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

 //   compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}

QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}


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

    //int cardWidth = 80;
    int cardHeight = 100;

    // Positionnez les nouvelles cartes de manière dynamique ici
    // Pour l'exemple, je les positionne à des coordonnées aléatoires
    int newX = QRandomGenerator::global()->bounded(width() - 80); // Nouvelles coordonnées X
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y
    newCard->setGeometry(newX, newY, 80, 100);


    newCard->show();

    prises.append(newCard);
}

// Fonction pour vérifier si un joueur a terminé
void cartes::checkEndGame() {
    int computerCardCount = 0;
    int playerCardCount = 0;

    // Compter les cartes restantes pour chaque joueur
    for (QLabel *card : prises) {
        if (card->property("isComputerCard").toBool()) {
            computerCardCount++;
        } else {
            playerCardCount++;
        }
    }

    // Vérifier si un joueur n'a plus de cartes et afficher le message approprié
    if (computerCardCount == 0) {
        // Afficher un message indiquant que le joueur a gagné
        QMessageBox::information(this, "Fin du jeu", "Le joueur a gagné!");
        // Arrêter le jeu ou effectuer d'autres actions nécessaires pour la fin du jeu
    } else if (playerCardCount == 0) {
        // Afficher un message indiquant que l'ordinateur a gagné
        QMessageBox::information(this, "Fin du jeu", "L'ordinateur a gagné!");
        // Arrêter le jeu ou effectuer d'autres actions nécessaires pour la fin du jeu
    }
}

cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}*/
/*
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
#include <QDebug>
#include <QResizeEvent>
#include <QMap>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>



// Création d'une sous-classe personnalisée de QLabel pour gérer les événements de souris sur les cartes
class CustomLabel : public QLabel {
public:
    explicit CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", true);
            this->setProperty("clickOffset", event->pos());
        }
        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (this->property("isBeingMoved").toBool()) {
            QPoint clickOffset = this->property("clickOffset").toPoint();
            int newX = event->globalPosition().x() - clickOffset.x();
            int newY = event->globalPosition().y() - clickOffset.y();
            this->move(newX, newY);
        }
        QLabel::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", false);
        }
        QLabel::mouseReleaseEvent(event);
    }
};







cartes::cartes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartes)
{
    ui->setupUi(this);

    prises = QVector<QLabel*>();
    gameStarted = false; // Initialisation du jeu comme non démarré

    // Créer le fond d'écran avec un bouton de démarrage
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QPixmap backgroundImage(":/k/logoSota.png");
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    centralWidget->setPalette(palette);

    QPushButton *startButton = new QPushButton("Commencer le jeux", centralWidget);
    startButton->setObjectName("startButton"); // Pour la définition du nom du bouton

    // j'ai crée la forme du bouton ici
    startButton->setStyleSheet("background-color:  #F5F5DC ; color: #A52A2A; padding: 8px; border-radius: 10px;");


    layout->addWidget(startButton, 0, Qt::AlignCenter);
    connect(startButton, &QPushButton::clicked, this, &cartes::startGame);

    setCentralWidget(centralWidget);
}


void cartes::startGame() {
    if (!gameStarted) {
        gameStarted = true; // Marquer le jeu comme démarré
        clearPreviousCards(); // Méthode pour effacer les cartes existantes
        addCards();

        // Mélanger les cartes
        std::random_shuffle(prises.begin(), prises.end());

        // Distribuer les cartes
        distributeCards();

        // Masquer le bouton une fois le jeu commencé
        QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
        if (startButton) {
            startButton->hide();
        }

        // Une fois le jeu commencé, masquez le fond d'écran
        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }
}

void cartes::compareAndMoveCards() {
    QVector<QLabel *> computerCards;
    QVector<QLabel *> playerCards;

    for (QLabel *card : prises) {
        if (card->property("isComputerCard").toBool()) {
            computerCards.append(card);
        } else {
            playerCards.append(card);
        }
    }

    //if (!computerCards.isEmpty() && !playerCards.isEmpty()) {
        //QLabel *computerCard = computerCards.at(QRandomGenerator::global()->bounded(computerCards.size()));
       //QLabel *playerCard = playerCards.at(QRandomGenerator::global()->bounded(playerCards.size()));

        // Ici, vous devez implémenter votre propre logique de comparaison des cartes
        // Si les cartes correspondent, déplacez-les au centre de l'interface
        // Sinon, ne faites rien pour le moment
    //}

}

void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear(); // Vide le vecteur de cartes
}


void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" << ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

   for (const QString &imagePath : cardImages) {
       processCardTake(imagePath);
    }
}


void cartes::distributeCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

     //QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffset = 0; // Offset horizontal pour les cartes
    bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        // QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

        QPropertyAnimation *moveAnimation = new QPropertyAnimation(card, "geometry");
        moveAnimation->setDuration(1000);

        int newX, newY;

        if (i % 2 == 0) { // Ligne du haut
            newX = horizontalOffset;
            newY = topY;
        } else { // Ligne du bas
            newX = horizontalOffset;
            newY = bottomY;
            horizontalOffset += cardWidth + spacing; // Augmenter l'offset horizontal pour la prochaine carte
        }


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
            QPixmap cardBackImage(computerCardBackImagePath);
            card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

    compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}

QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}


void cartes::processCardTake(const QString& imagePath)
{
    CustomLabel* newCard = new CustomLabel(this); // Utilisation de CustomLabel au lieu de QLabel

    QPixmap cardImage(imagePath);

    if (cardImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de la carte.";
        return;
    }

    cardImage = cardImage.scaled(80, 100, Qt::KeepAspectRatio);

    newCard->setPixmap(cardImage);

    //int cardWidth = 80;
    int cardHeight = 100;

    // Positionnez les nouvelles cartes de manière dynamique ici
    // Pour l'exemple, je les positionne à des coordonnées aléatoires
    int newX = QRandomGenerator::global()->bounded(width() - 80); // Nouvelles coordonnées X
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y
    newCard->setGeometry(newX, newY, 80, 100);


    newCard->show();

    prises.append(newCard);
}


cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}
*/
/*
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
#include <QDebug>
#include <QResizeEvent>
#include <QMap>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>



// Création d'une sous-classe personnalisée de QLabel pour gérer les événements de souris sur les cartes
class CustomLabel : public QLabel {
public:
    explicit CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", true);
            this->setProperty("clickOffset", event->pos());
        }
        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (this->property("isBeingMoved").toBool()) {
            QPoint clickOffset = this->property("clickOffset").toPoint();
            int newX = event->globalPosition().x() - clickOffset.x();
            int newY = event->globalPosition().y() - clickOffset.y();
            this->move(newX, newY);
        }
        QLabel::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", false);
        }
        QLabel::mouseReleaseEvent(event);
    }
};



cartes::cartes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartes)
{
    ui->setupUi(this);

    prises = QVector<QLabel*>();
    gameStarted = false; // Initialisation du jeu comme non démarré

    // Créer le fond d'écran avec un bouton de démarrage
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QPixmap backgroundImage(":/k/logoSota.png"); // Remplacez par le chemin de votre image de fond
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    centralWidget->setPalette(palette);

    QPushButton *startButton = new QPushButton("Commencer le jeux", centralWidget);
    startButton->setObjectName("startButton"); // Pour la définition du nom du bouton

    // j'ai crée la forme du bouton ici
    startButton->setStyleSheet("background-color:  #F5F5DC ; color: #A52A2A; padding: 8px; border-radius: 10px;");


    layout->addWidget(startButton, 0, Qt::AlignCenter);
    connect(startButton, &QPushButton::clicked, this, &cartes::startGame);

    setCentralWidget(centralWidget);
}

void cartes::startGame() {
    if (!gameStarted) {
        gameStarted = true; // Marquer le jeu comme démarré
        clearPreviousCards(); // Méthode pour effacer les cartes existantes
        addCards();

        // Mélanger les cartes
        std::random_shuffle(prises.begin(), prises.end());

        // Distribuer les cartes
        distributeCards();

        // Masquer le bouton une fois le jeu commencé
      QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
        if (startButton) {
            startButton->hide();
        }

        // Une fois le jeu commencé, masquez le fond d'écran
        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }
}

void cartes::compareAndMoveCards() {
    QVector<QLabel *> computerCards;
    QVector<QLabel *> playerCards;

    for (QLabel *card : prises) {
        if (card->property("isComputerCard").toBool()) {
            computerCards.append(card);
        } else {
            playerCards.append(card);
        }
    }

    if (!computerCards.isEmpty() && !playerCards.isEmpty()) {
       // QLabel *computerCard = computerCards.at(QRandomGenerator::global()->bounded(computerCards.size()));
      //  QLabel *playerCard = playerCards.at(QRandomGenerator::global()->bounded(playerCards.size()));

        // Ici, vous devez implémenter votre propre logique de comparaison des cartes
        // Si les cartes correspondent, déplacez-les au centre de l'interface
        // Sinon, ne faites rien pour le moment
    }
}

void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear(); // Vide le vecteur de cartes
}


void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" << ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

    for (const QString &imagePath : cardImages) {
        processCardTake(imagePath);
    }
}


void cartes::distributeCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

    // QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffset = 0; // Offset horizontal pour les cartes
      bool showBack = true; // Variable pour contrôler l'affichage du dos de la carte

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        // QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

        QPropertyAnimation *moveAnimation = new QPropertyAnimation(card, "geometry");
        moveAnimation->setDuration(1000);

        int newX, newY;

        if (i % 2 == 0) { // Ligne du haut
            newX = horizontalOffset;
            newY = topY;
        } else { // Ligne du bas
            newX = horizontalOffset;
            newY = bottomY;
            horizontalOffset += cardWidth + spacing; // Augmenter l'offset horizontal pour la prochaine carte
        }


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Vérifier si la carte doit afficher son dos ou sa face
        if (showBack) {
            QPixmap cardBackImage(computerCardBackImagePath);
            card->setPixmap(cardBackImage);
        }

        showBack = !showBack; // Inverser l'état pour la prochaine carte

        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

    compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}

QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}


void cartes::processCardTake(const QString& imagePath)
{
    CustomLabel* newCard = new CustomLabel(this); // Utilisation de CustomLabel au lieu de QLabel

    QPixmap cardImage(imagePath);

    if (cardImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de la carte.";
        return;
    }

    cardImage = cardImage.scaled(80, 100, Qt::KeepAspectRatio);

    newCard->setPixmap(cardImage);

    int cardWidth = 80;
    int cardHeight = 100;
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y


    bool isComputerCard = QRandomGenerator::global()->bounded(2) == 0;

    if (isComputerCard) {
        newCard->setProperty("isComputerCard", true);
        QPixmap cardBackImage(computerCardBackImagePath);
        newCard->setPixmap(cardBackImage);
    } else {
        newCard->setProperty("isComputerCard", false);
    }

    int newX = QRandomGenerator::global()->bounded(width() - cardWidth); // Nouvelles coordonnées X
    newCard->setGeometry(newX, newY, cardWidth, cardHeight);
    newCard->show();

    prises.append(newCard);
}

cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}
*/






/*
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QParallelAnimationGroup>
#include <QtGlobal>
#include <cstdlib>
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
#include <QDebug>
#include <QResizeEvent>
#include <QMap>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>



// Création d'une sous-classe personnalisée de QLabel pour gérer les événements de souris sur les cartes
class CustomLabel : public QLabel {
public:
    explicit CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", true);
            this->setProperty("clickOffset", event->pos());
        }
        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (this->property("isBeingMoved").toBool()) {
            QPoint clickOffset = this->property("clickOffset").toPoint();
            int newX = event->globalPosition().x() - clickOffset.x();
            int newY = event->globalPosition().y() - clickOffset.y();
            this->move(newX, newY);
        }
        QLabel::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->setProperty("isBeingMoved", false);
        }
        QLabel::mouseReleaseEvent(event);
    }
};



cartes::cartes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cartes)
{
    ui->setupUi(this);

    prises = QVector<QLabel*>();
    gameStarted = false; // Initialisation du jeu comme non démarré

    // Créer le fond d'écran avec un bouton de démarrage
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QPixmap backgroundImage(":/k/logoSota.png"); // Remplacez par le chemin de votre image de fond
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    centralWidget->setPalette(palette);

    QPushButton *startButton = new QPushButton("Commencer le jeux", centralWidget);
    startButton->setObjectName("startButton"); // Pour la définition du nom du bouton

    // j'ai crée la forme du bouton ici
    startButton->setStyleSheet("background-color:  #F5F5DC ; color: #A52A2A; padding: 8px; border-radius: 10px;");


    layout->addWidget(startButton, 0, Qt::AlignCenter);
    connect(startButton, &QPushButton::clicked, this, &cartes::startGame);

    setCentralWidget(centralWidget);
}

void cartes::startGame() {
    if (!gameStarted) {
        gameStarted = true; // Marquer le jeu comme démarré
        clearPreviousCards(); // Méthode pour effacer les cartes existantes
        addCards();

        // Mélanger les cartes
        std::random_shuffle(prises.begin(), prises.end());

        // Distribuer les cartes
        distributeCards();

        // Masquer le bouton une fois le jeu commencé
      QPushButton *startButton = qobject_cast<QPushButton*>(this->centralWidget()->findChild<QObject*>("startButton"));
        if (startButton) {
            startButton->hide();
        }

        // Une fois le jeu commencé, masquez le fond d'écran
        QPixmap newBackground(":/k/bois3.png");
        this->centralWidget()->setAutoFillBackground(true);
        QPalette newPalette;
        newPalette.setBrush(QPalette::Window, newBackground);
        this->centralWidget()->setPalette(newPalette);
    }
}

void cartes::compareAndMoveCards() {
    QVector<QLabel *> computerCards;
    QVector<QLabel *> playerCards;

    for (QLabel *card : prises) {
        if (card->property("isComputerCard").toBool()) {
            computerCards.append(card);
        } else {
            playerCards.append(card);
        }
    }

    if (!computerCards.isEmpty() && !playerCards.isEmpty()) {
       // QLabel *computerCard = computerCards.at(QRandomGenerator::global()->bounded(computerCards.size()));
      //  QLabel *playerCard = playerCards.at(QRandomGenerator::global()->bounded(playerCards.size()));

        // Ici, vous devez implémenter votre propre logique de comparaison des cartes
        // Si les cartes correspondent, déplacez-les au centre de l'interface
        // Sinon, ne faites rien pour le moment
    }
}

void cartes::clearPreviousCards() {
    for (QLabel* card : prises) {
        delete card;
    }
    prises.clear(); // Vide le vecteur de cartes
}


void cartes::addCards() {
    QStringList cardImages;
    cardImages << ":/k/D1.jpg" << ":/k/D11.jpg" << ":/k/D12.jpg" <<":/k/D2.jpg" <<":/k/D3.jpg"<<":/k/D4.jpg"<<":/k/D5.jpg"<<":/k/D6.jpg"<<":/k/D7.jpg"
               <<":/k/G1.jpg"<<":/k/G10.jpg"<<":/k/G11.jpg"<<":/k/G12.jpg"<<":/k/G2.jpg"<<":/k/G3.jpg"<<":/k/G4.jpg"<<":/k/G5.jpg"<<":/k/G6.jpg"<<":/k/G7.jpg"
               <<":/k/S1.jpg"<<":/k/S11.jpg"<<":/k/S12.jpg"<<":/k/S2.jpg"<<":/k/S3.jpg"<<":/k/S4.jpg"<<":/k/S5.jpg"<<":/k/S6.jpg"<<":/k/S7.jpg"
               <<":/k/Z1.jpg"<<":/k/Z11.jpg"<<":/k/Z12.jpg"<<":/k/Z2.jpg"<<":/k/Z3.jpg"<<":/k/Z4.jpg"<<":/k/Z5.jpg"<<":/k/Z6.jpg"<<":/k/Z7.jpg";

    // Mélangez la liste d'images de cartes
    std::random_shuffle(cardImages.begin(), cardImages.end());

    for (const QString &imagePath : cardImages) {
        processCardTake(imagePath);
    }
}


void cartes::distributeCards() {
    int cardWidth = 80;
    int cardHeight = 100;
    int spacing = -50; // Espacement horizontal entre les cartes
    int topY = 0; // Position Y de la ligne du haut
    static int bottomY = height() - cardHeight-50; // Position Y de la ligne du bas

    // QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup(this);

    // QRandomGenerator randomGenerator(QDateTime::currentMSecsSinceEpoch());

    std::random_shuffle(prises.begin(), prises.end()); // Mélanger l'ordre des cartes

    int horizontalOffset = 0; // Offset horizontal pour les cartes

    // Distribuer les cartes en boucle
    for (int i = 0; i < prises.size(); ++i) {
        QLabel *card = prises.at(i);
        // QString cardValue = extractCardValue(card->text()); // Extraire la valeur de la carte

        QPropertyAnimation *moveAnimation = new QPropertyAnimation(card, "geometry");
        moveAnimation->setDuration(1000);

        int newX, newY;

        if (i % 2 == 0) { // Ligne du haut
            newX = horizontalOffset;
            newY = topY;
        } else { // Ligne du bas
            newX = horizontalOffset;
            newY = bottomY;
            horizontalOffset += cardWidth + spacing; // Augmenter l'offset horizontal pour la prochaine carte
        }


        moveAnimation->setStartValue(card->geometry());
        moveAnimation->setEndValue(QRect(newX, newY, cardWidth, cardHeight));
        moveAnimation->setEasingCurve(QEasingCurve::InOutQuad);


        moveAnimation->start(); // Démarrer l'animation pour chaque carte individuellement
    }

    compareAndMoveCards(); // Appeler la fonction pour comparer et déplacer les cartes
}

QString cartes::extractCardValue(const QString &imageName) {
    // Extraire la valeur de la carte à partir du nom de l'image
    QString cardValue = imageName.mid(3, imageName.indexOf('.') - 3); // Supposant que le nom de l'image est dans le format XYZ.jpg (ex: D10.jpg)
    return cardValue ;
}


void cartes::processCardTake(const QString& imagePath)
{
    CustomLabel* newCard = new CustomLabel(this); // Utilisation de CustomLabel au lieu de QLabel

    QPixmap cardImage(imagePath);

    if (cardImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image de la carte.";
        return;
    }

    cardImage = cardImage.scaled(80, 100, Qt::KeepAspectRatio);

    newCard->setPixmap(cardImage);

    int cardWidth = 80;
    int cardHeight = 100;
    int newY = QRandomGenerator::global()->bounded(height() - cardHeight); // Nouvelles coordonnées Y


    bool isComputerCard = QRandomGenerator::global()->bounded(2) == 0;

    if (isComputerCard) {
        newCard->setProperty("isComputerCard", true);
        QPixmap cardBackImage(computerCardBackImagePath);
        newCard->setPixmap(cardBackImage);
    } else {
        newCard->setProperty("isComputerCard", false);
    }

    int newX = QRandomGenerator::global()->bounded(width() - cardWidth); // Nouvelles coordonnées X
    newCard->setGeometry(newX, newY, cardWidth, cardHeight);
    newCard->show();

    prises.append(newCard);
}

cartes::~cartes()
{
    clearPreviousCards(); // Assurez-vous de nettoyer les cartes restantes à la destruction de l'objet
    delete ui;
}

*/
