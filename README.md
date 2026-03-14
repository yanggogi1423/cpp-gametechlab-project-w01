> ### Krafton Jungle GameTechLab 3기 - Week01 3조 Team PUSHFORCE

</br>

# 🚀 SPACE.EXE

## 프로젝트 개요

* **장르** : 물리 기반 퍼즐 게임
* **플랫폼** : PC (Windows)
* **개발 기간** : 2026.03.09 – 2026.03.12

### 팀 구성

| 이름  | 역할          |
| --- | ----------- |
| 김기훈 | PM / Planet, Sounds |
| 박상혁 | Probe, Frameworks       |
| 양현석 | Management, UI  |
| 한영서 | Renderer    |

</br>

---

## 게임 설명

“불안정한 항성계를 탈출하기 위한 최후의 탐사선을 위해, 당신은 지상에서 궤도를 계산하는 엔지니어입니다.”

플레이어는 2차원 우주 공간에 행성들을 배치하여 탐사선이 목표 지점에 도달할 수 있도록 궤도를 설계해야 합니다.
행성의 중력을 이용한 **스윙바이(Swing-by) 항법**을 활용하여 탐사선의 경로를 조정하는 것이 핵심 게임 메커니즘입니다. 

</br>

---

## 게임 목표

탐사선이 **제한 시간 안에 다른 천체에 충돌하지 않고 목표 지점에 도달하면 스테이지 클리어**입니다.

다음 상황에서는 게임 오버가 됩니다.

* 탐사선이 화면 밖으로 이탈한 경우
* 행성과 충돌한 경우
* 제한 시간 안에 목표 지점에 도달하지 못한 경우

</br>

---

## 게임 진행 방식

게임은 여러 개의 스테이지로 구성되어 있으며, 이전 스테이지를 클리어해야 다음 스테이지를 플레이할 수 있습니다.
스테이지 번호가 올라갈수록 난이도도 증가합니다.

각 스테이지에는 **사용해야 하는 행성의 종류와 개수에 대한 제약 조건**이 존재하며,
플레이어는 반드시 주어진 행성을 모두 사용하여 우주 공간을 설계해야 합니다.

플레이 흐름은 다음과 같습니다.

1. 행성을 우주 공간에 배치
2. `Start` 버튼을 눌러 탐사선 발사
3. 탐사선의 궤도를 확인
4. 목표 지점 도달 시 스테이지 클리어

</br>

---

## 게임 오브젝트

| 오브젝트       | 설명                       |
| ---------- | ------------------------ |
| Probe      | 플레이어가 목표 지점으로 보내야 하는 탐사선 |
| Planet     | 중력을 발생시키는 천체             |
| Goal       | 탐사선이 도달해야 하는 목표 지점       |

</br>

---

## 스토리

우주 미아가 될 위기에 처한 우주비행사 **존(John)**이 있습니다.

당신이 지상의 엔지니어로서 올바른 궤도를 설계하지 못한다면
그는 우주에서 영원히 길을 잃을 수도 있습니다.

</br>

---

# English Version

## Project Overview

* **Genre** : Physics-Based Puzzle Game
* **Platform** : PC (Windows)
* **Development Period** : 2026.03.09 – 2026.03.12

### Team

| Name          | Role        |
| ------------- | ----------- |
| Kihoon Kim    | PM / Planet, Sounds |
| Sanghyuk Park | Probe, Frameworks       |
| Hyunseok Yang | Management, UI  |
| Youngseo Han  | Renderer    |

</br>

---

## Game Description

“You are an engineer on the ground calculating orbital trajectories for the last probe escaping an unstable star system.”

Players design trajectories by placing planets in a **2D space environment** so that the probe can reach the target location.
The core gameplay mechanic is controlling the probe’s path using **gravity and swing-by maneuvers**. 

</br>

---

## Game Goal

A stage is cleared when the probe **reaches the target location within the time limit without colliding with other celestial objects.**

The game ends in failure when:

* The probe leaves the screen
* The probe collides with a planet
* The probe fails to reach the target within the time limit

</br>

---

## Gameplay

The game consists of multiple stages, and players must clear the previous stage to unlock the next one.
As the stage number increases, the difficulty also increases.

Each stage has **constraints on the types and number of planets that must be used**,
and players must place all provided planets to design the space environment.

Gameplay flow:

1. Place planets in space
2. Press the `Start` button to launch the probe
3. Observe the trajectory
4. Reach the target to clear the stage

</br>

---

## Game Objects

| Object     | Description                                           |
| ---------- | ----------------------------------------------------- |
| Probe      | The spacecraft that the player must guide to the goal |
| Planet     | Celestial bodies that generate gravitational force    |
| Goal       | The destination that the probe must reach             |

</br>

---

## Story

An astronaut named **John** is on the verge of becoming lost in space.

If you fail to design the correct orbital trajectory from the ground,
he may never find his way back.

</br>

---

