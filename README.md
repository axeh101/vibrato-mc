# Calcul des sensibilités : méthode hybride Vibrato Monte Carlo et différentiation automatique

## Objectif

1. Étudier la méthode numérique introduit dans [PPS15] (cf. aussi [Gil09]) pour calculer des sensibilités du premier
   ordre (delta, vega) et du second ordre par la méthode de Vibrato Monte Carlo (qui consiste à régulariser les payoffs)
   .
2. Vérifier numériquement l’approche en insistant sur le coût numérique (la complexité). Il est possible d’appeler ou
   d’adapter des librairies existantes de différentiation automatique (cf. le site http://www.autodiff.org)

Références: [PPS15], [Gil09].

[[PPS15](https://arxiv.org/abs/1606.06143)] Vibrato and automatic differentiation for high order derivatives and
sensitivities of financial options

[[Gil09](https://link.springer.com/chapter/10.1007/978-3-642-04107-5_23)] Vibrato Monte Carlo Sensitivities

## Prérequis

Pour la différentiation automatique, on utilise la librairie `autodiff`. Ci-dessous les étapes d'installation:

1. Téléchargement de la librairie:

    - En utilisant git
      ```
      git clone https://github.com/autodiff/autodiff
      ```
    - En téléchargeant directement le fichier zip [ici](https://github.com/autodiff/autodiff/archive/master.zip).

2. Installation de la librairie:
   Cela requière des droits administrateurs. Il est aussi possible d'installer la librarie localement, il suffit de
   remplacer dans les commandes ci-dessous `cmake ..` par `cmake .. -DCMAKE_INSTALL_PREFIX=directory`, où _"directory"_
   correspond a un endroit de votre choix par exemple `cmake .. - l` et sans le `sudo`
   à la dernière commande.

```bash
cd autodiff
mkdir .build && cd .build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
sudo cmake --build . --target install
```

La librairie requière aussi la librairie `eigne3`. Pour l'installer il suffit:

- Sous MacOs le plus simple est de lancer la commande `brew install eigen`
- Sous Linux :
    - Fedora : `sudo dnf install eigen3-devel`
    - Debian : `sudo apt install libeigen3-dev`

Pour plus d'information, consulter le site web [autodiff](https://autodiff.github.io/) et
[eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)

## Compilation & lancement des programmes

Un fichier `Makefile` contient toutes les commandes déjà prédéfinies. Tous les binaires sont dans le
dossier `vibrato-mc/bin`. Plusieurs options sont possibles:

1. Compiler tous les fichiers:

```bash
make all
```

2. Compilation et génération des sorties pour la méthode Vibrato:

    - Pour une option vanille exécuter ```make vanilla``` puis `bin/./vibrato-vanilla`
    - Pour une option binaire exécuter ```make digital``` puis `bin/./vibrato-digital`
    - Pour le modèle de Heston exécuter ```make heston``` puis `bin/./vibrato-heston`


3. Compilation et génération des sorties pour la méthode Vibrato avec différentiation automatique :

    - Pour une option vanille exécuter ```make vanilla-ad``` puis `bin/./vad-vanilla`
    - Pour une option binaire exécuter ```make digital-ad``` puis `bin/./vad-digital`
    - Pour le modèle de Heston exécuter ```make heston-ad``` puis `bin/./vad-heston`


4. pour les sorties des mesure de performances:

    - Executer ```make perf``` puis `bin/./vibrato-perf`

# Exemple de sortie du programme de calcul :

                              **************** Pricing results ****************
                              *************************************************
                              Pricing Vanilla option Analytic vs Vibrato Heston
                              *************************************************
      *************** Black-Scholes **************     ************* Vibrato (Heston) ****************
      Name: Vanilla Option                                  Name: Vanilla Option
      Option Type: Call                                     Option Type: Call
      Maturity: 1                                           Maturity: 1
      Strike: 100                                           Strike: 100
      
      Process name: Black-Scholes                           Process name: Heston
   
      Rate: 0.05                                            Rate: 0.05
      Volatility: 0.32                                      Volatility:
                                                               Process name: Cox-Ingeresoll-Ross
                                                               Rate: 2.93147
                                                               Kappa:2.93147
                                                               Eta:0.101
                                                               Vol Vol:0.05
                                                               Correlation:0.5
      ************************************************************************************************
      Premium: 14.9902                                     Premium: 15.0166
      Delta: 0.624094                                      Delta: 0.559846
      Gamma: 0.0118588                                     Gamma: 0.0141318
      Vega: 37.9483                                        Vega: 30.8113
      Vanna: 0.00444707                                    Vanna: 0.336125
      Volga: -0.140638                                     Volga: 248.548
      Rho: 47.4192                                         Rho: 44.8867
      Theta: -8.44269                                      Theta: -0.717607




                              *************************************************
                              Pricing Digital option Analytic vs Vibrato Heston
                              *************************************************

      ************ Black-Scholes ****************     ************** Vibrato (Heston) ****************
      Name: Digital Option                               Name: Digital Option
      Option Type: Call                                  Option Type: Call
      Maturity: 1                                        Maturity: 1
      Strike: 100                                        Strike: 100
      Price: 100                                         Price: 100
      Rate: 0.05                                         Rate: 0.05
      Volatility: 0.32                                   Volatility:
                                                            Process name: Cox-Ingeresoll-Ross
                                                            Initial vol: 0.028087
                                                            Rate: 2.93147
                                                            Kappa:2.93147
                                                            Eta:0.101
                                                            Vol Vol:0.05
                                                            Correlation:0.5
      ************************************************************************************************
      Premium: 0.474192                                  Premium: 0.477912
      Delta: 0.0118588                                   Delta: 0.0117135
      Gamma:  0.000117199                                Gamma: 0.000114066
      Vega: -0.375036                                    Vega: -0.241489
      Vanna: -0.0371028                                  Vanna: -0.0433652
      Volga: 1.15948                                     Volga: 6.33861
      Rho: 0.711693                                      Rho: 0.737581
      Theta: -0.0244211                                  Theta: -0.0222913

                              *********************~END~***********************