            struct UpdateDirectIndirectReflectionColourFunctor
            {
                typedef OneOrMany<Shader::Sample*> GlossySamplesContainer;

                UpdateDirectIndirectReflectionColourFunctor( Colour& totalReflectionColour,
                    Colour& totalIndirectReflectionColour,
                    const Colour& aCoatingColour,
                    GlossySamplesContainer::const_iterator bgSample_it,
                    float aKrefl_refl)
                    :
                coatingColour(aCoatingColour)
                {
                    krefl_refl = aKrefl_refl;
                    if(bgSample_it != NULL)
                        hasBGSample = true;
                    else
                        hasBGSample = false;
                }

                void operator()( Salix::Shader::Sample*& refSample_it )
                {

                    Shader::Sample * samplePtr = (hasBGSample) ? *bgSample_it : NULL;
                    boost::tie(reflectionColour, indirectReflectionColour) = ComputeReflection(krefl_refl, coatingColour, refSample_it, samplePtr);

                    totalReflectionColour += reflectionColour;
                    totalIndirectReflectionColour += indirectReflectionColour;

                    if( hasBGSample ) ++bgSample_it;
                }


                float krefl_refl;
                const Colour& coatingColour;
                Colour reflectionColour;
                Colour indirectReflectionColour;  
                GlossySamplesContainer::const_iterator bgSample_it;
                bool hasBGSample;

            };