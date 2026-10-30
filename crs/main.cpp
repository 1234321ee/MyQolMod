#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

// ============================================================
// 1. FPS, СЧЁТЧИК ПОПЫТОК И КНОПКА РЕСТАРТА
// ============================================================
class $modify(MyPlayLayer, PlayLayer) {
    CCLabelBMFont* m_fpsLabel = nullptr;
    CCLabelBMFont* m_attemptsLabel = nullptr;

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // --- FPS ---
        m_fpsLabel = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
        m_fpsLabel->setScale(0.35f);
        m_fpsLabel->setAnchorPoint({0.f, 1.f});
        m_fpsLabel->setPosition({8.f, winSize.height - 8.f});
        m_fpsLabel->setZOrder(1000);
        this->addChild(m_fpsLabel);

        // --- Счётчик попыток ---
        m_attemptsLabel = CCLabelBMFont::create("Attempts: 0", "bigFont.fnt");
        m_attemptsLabel->setScale(0.35f);
        m_attemptsLabel->setAnchorPoint({0.f, 1.f});
        m_attemptsLabel->setPosition({8.f, winSize.height - 30.f});
        m_attemptsLabel->setZOrder(1000);
        this->addChild(m_attemptsLabel);

        // --- Кнопка рестарта ---
        auto restartSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
        if (!restartSpr) restartSpr = ButtonSprite::create("R");
        
        auto restartBtn = CCMenuItemSpriteExtra::create(
            restartSpr, this, menu_selector(MyPlayLayer::onRestart)
        );
        
        auto restartMenu = CCMenu::create();
        restartMenu->addChild(restartBtn);
        restartMenu->setPosition({winSize.width - 30.f, 30.f});
        restartMenu->setZOrder(1000);
        this->addChild(restartMenu);

        return true;
    }

    void onRestart(CCObject* sender) {
        PlayLayer::resetLevel();
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (m_fpsLabel) {
            int fps = static_cast<int>(1.f / dt);
            m_fpsLabel->setString(fmt::format("FPS: {}", fps).c_str());
        }

        if (m_attemptsLabel && this->m_level) {
            int attempts = this->m_level->m_attempts;
            m_attemptsLabel->setString(fmt::format("Attempts: {}", attempts).c_str());
        }
    }
};

// ============================================================
// 2. ИМЯ СОЗДАТЕЛЯ В МЕНЮ ПАУЗЫ
// ============================================================
class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        std::string creator = pl->m_level->m_creatorName;
        if (creator.empty()) creator = "Unknown";

        auto label = CCLabelBMFont::create(
            fmt::format("Creator: {}", creator).c_str(), 
            "bigFont.fnt"
        );
        label->setScale(0.5f);
        label->setPosition({CCDirector::sharedDirector()->getWinSize().width / 2.f, 40.f});
        label->setZOrder(100);
        this->addChild(label);
    }
};

// ============================================================
// 3. ТЕКСТ ПРИ ПРОХОЖДЕНИИ УРОВНЯ
// ============================================================
class $modify(MyEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        bool isPractice = false;
        if (auto pl = PlayLayer::get()) {
            isPractice = pl->m_isPracticeMode;
        }

        std::string message;
        if (isPractice) {
            message = "Хоть ты прошел и с практикой, но ты полный молодец! Не останавливайся!";
        } else {
            message = "Молодец! Ты сможешь лучше! Не заканчивай!";
        }

        FLAlertLayer::create(
            "Поздравляем!", 
            message.c_str(), 
            "Спасибо!"
        )->show();
    }
};
