#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static int fakeDeaths = 0;

class $modify(FakeDeathPlayLayer, PlayLayer) {
    CCLabelBMFont* m_deathLabel = nullptr;

    bool init(GJGameLevel* level) {
        if (!PlayLayer::init(level))
            return false;

        m_deathLabel = CCLabelBMFont::create(
            "Deaths: 0",
            "goldFont.fnt"
        );

        m_deathLabel->setAnchorPoint({0.f, 1.f});
        m_deathLabel->setPosition({8.f,
            CCDirector::sharedDirector()
                ->getWinSize().height - 8.f});

        m_uiLayer->addChild(m_deathLabel, 999);

        schedule(schedule_selector(FakeDeathPlayLayer::updateFakeDeaths));

        return true;
    }

    void updateFakeDeaths(float) {
        auto player = m_player1;

        if (!player)
            return;

        auto pos = player->getPosition();

        bool nearSpike = false;

        auto objects = m_objects;

        for (auto obj : CCArrayExt<GameObject*>(objects)) {

            int id = obj->m_objectID;

            bool hazard =
                id == 8 ||
                id == 39 ||
                id == 103 ||
                id == 392;

            if (!hazard)
                continue;

            float dx =
                obj->getPositionX() - pos.x;

            float dy =
                obj->getPositionY() - pos.y;

            float dist =
                sqrtf(dx * dx + dy * dy);

            if (dist < 120.f) {
                nearSpike = true;
                break;
            }
        }

        if (nearSpike) {
            fakeDeaths++;

            m_deathLabel->setString(
                fmt::format(
                    "Deaths: {}",
                    fakeDeaths
                ).c_str()
            );
        }
    }
};
