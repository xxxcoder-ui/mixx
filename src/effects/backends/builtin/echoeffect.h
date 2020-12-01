#pragma once

#include <QMap>

#include "effects/backends/effectprocessor.h"
#include "engine/engine.h"
#include "engine/effects/engineeffect.h"
#include "engine/effects/engineeffectparameter.h"
#include "util/class.h"
#include "util/defs.h"
#include "util/sample.h"
#include "util/samplebuffer.h"

class EchoGroupState : public EffectState {
  public:
    // 3 seconds max. This supports the full range of 2 beats for tempos down to
    // 40 BPM.
    static constexpr int kMaxDelaySeconds = 3;

    EchoGroupState(const mixxx::EngineParameters& bufferParameters)
            : EffectState(bufferParameters) {
        audioParametersChanged(bufferParameters);
       clear();
    }

    void audioParametersChanged(const mixxx::EngineParameters& bufferParameters) {
        delay_buf = mixxx::SampleBuffer(kMaxDelaySeconds
                * bufferParameters.sampleRate() * bufferParameters.channelCount());
    };

    void clear() {
        delay_buf.clear();
        prev_send = 0.0f;
        prev_feedback= 0.0f;
        prev_delay_samples = 0;
        write_position = 0;
        ping_pong = 0;
    };

    mixxx::SampleBuffer delay_buf;
    CSAMPLE_GAIN prev_send;
    CSAMPLE_GAIN prev_feedback;
    int prev_delay_samples;
    int write_position;
    int ping_pong;
};

class EchoEffect : public EffectProcessorImpl<EchoGroupState> {
  public:
    EchoEffect() = default;

    static QString getId();
    static EffectManifestPointer getManifest();

    void loadEngineEffectParameters(
            const QMap<QString, EngineEffectParameterPointer>& parameters) override;

    void processChannel(
            EchoGroupState* pState,
            const CSAMPLE* pInput, CSAMPLE* pOutput,
            const mixxx::EngineParameters& bufferParameters,
            const EffectEnableState enableState,
            const GroupFeatureState& groupFeatures) override;

  private:
    QString debugString() const {
        return getId();
    }

    EngineEffectParameterPointer m_pDelayParameter;
    EngineEffectParameterPointer m_pSendParameter;
    EngineEffectParameterPointer m_pFeedbackParameter;
    EngineEffectParameterPointer m_pPingPongParameter;
    EngineEffectParameterPointer m_pQuantizeParameter;
    EngineEffectParameterPointer m_pTripletParameter;

    DISALLOW_COPY_AND_ASSIGN(EchoEffect);
};
