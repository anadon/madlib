#include <string.h>


  /* Label/Key         : ^PLATFORM
   * Number per file   : 1
   * Value constraints : Unique key, ASCII
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records. 
   ********************************************************************/
int parse_PLATFORM(const char* toParse, struct GEOSimpleFile *parseTo){
  if(NULL != parseTo->PLATFORM) return -EINVAL;

  char *line = strdup(toParse);
  char *lineMemoryPtr = line;
  
  line += strlen("PLATFORM");
  while(*line == ' ' || *line == '\t') line++; 
  
  if(!strcmp("", line)) return -EINVAL;
  
  parseTo->PLATFORM = strdup(line);
  
  free(lineMemoryPtr);
  
  return 0;
}

  /* Label/Key         : !Platform_title
   * Number per file   : 1
   * Value constraints : string of length 1-120 characters, must be 
   * unique within local file and over all previously submitted 
   * Platforms for that submitter.
   * 
   * Notes: Provide a unique title that describes 
   * your Platform. We suggest that you use the system 
   * [institution/lab]-[species]-[number of features]-[version], e.g. 
   * "FHCRC Mouse 15K v1.0".
   ********************************************************************/
int parse_Platform_title(const char* toParse, struct GEOSimpleFile *parseTo){
  if(NULL != parseTo->Platform_title) return -EINVAL;

  char *line = strdup(toParse);
  char *lineMemoryPtr = line;
  
  line += strlen("Platform_title");
  while(*line == ' ' || *line == '\t') line++; 
  
  if(!strcmp("", line)) return -EINVAL;
  
  parseTo->Platform_title = strdup(line);
  
  free(lineMemoryPtr);
  
  return 0;
}

  /* Label/Key         : !Platform_distribution
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, 
   *                     custom-commercial}
   * 
   * Notes: Should be in accordance with how the array was manufactured. 
   * Use 'virtual' only if creating a virtual definition for MPSS, 
   * SARST, or RT-PCR data.
   ********************************************************************/
int parse_Platform_distribution(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_technology
   * Number per file   : 1
   * Value constraints : in {spotted DNA/cDNA, spotted oligonucleotide, 
   *                     in situ oligonucleotide, antibody, tissue, 
   *                     SARST, RT-PCR, or MPSS}
   * 
   * Notes: Select the category that best describes the Platform 
   * technology.
   ********************************************************************/
int parse_Platform_technology(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_organism
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Identify the organism(s) from which the features on the 
   * Platform were designed or derived.
   ********************************************************************/
int parse_Platform_organism(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_manufacturer
   * Number per file   : 1
   * Value constraints : ASCII
   * 
   * Notes: Provide the name of the company, facility or laboratory 
   * where the array was manufactured or produced.
   ********************************************************************/
int parse_Platform_manufacturer(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_manufacture_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Describe the array manufacture protocol. Include as much 
   * detail as possible, e.g., clone/primer set identification and 
   * preparation, strandedness/length, arrayer hardware/software, 
   * spotting protocols. You can include as much text as you need to 
   * thoroughly describe the protocol; it is strongly recommended that 
   * complete protocol descriptions are provided within your submission.
   ********************************************************************/
int parse_Platform_manufacture_protocol(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_catalog_number
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes:Provide the manufacturer catalog number for 
   * commercially-available arrays.
   ********************************************************************/
int parse_Platform_catalog_number(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_web_link
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, valid URL
   * 
   * Notes: Specify a Web link that directs users to supplementary 
   * information about the array. Please restrict to Web sites that you 
   * know are stable.
   ********************************************************************/
int parse_Platform_web_link(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_support
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   * 
   * Notes: Provide the surface type of the array, e.g., glass, 
   * nitrocellulose, nylon, silicon, unknown.
   ********************************************************************/
int parse_Platform_support(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_coating
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   * 
   * Notes: Provide the coating of the array, e.g., aminosilane, quartz, 
   * polysine, unknown.
   ********************************************************************/
int parse_Platform_coating(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_description
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Provide any additional descriptive information not captured 
   * in another field, e.g., array and/or feature physical dimensions, 
   * element grid system.
   ********************************************************************/
int parse_Platform_description(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_contributor
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, List all people associated with this 
   *                     array design.
   * 
   * Notes: Each value in the form 'firstname,middleinitial,lastname' or 
   * 'firstname,lastname': firstname must be at least one character and 
   * cannot contain spaces; middleinitial, if present, is one character; 
   * lastname is at least two characters and can contain spaces. 
   ********************************************************************/
int parse_Platform_contributor(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_pubmed_id
   * Number per file   : [0, Infinity)
   * Value constraints : Integer, see notes.
   * 
   * Notes: Specify a valid PubMed identifier (PMID) that references a 
   * published article that describes the array.
   ********************************************************************/
int parse_Platform_pubmed_id(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Integer, see notes.
   * 
   * Notes: A valid Platform accession number (GPLxxx) Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
int parse_Platform_geo_accession(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : ^SAMPLE
   * Number per file   : 1
   * Value constraints : ASCII, Unique within file
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records.
   ********************************************************************/
int parse_SAMPLE(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_title
   * Number per file   : 1
   * Value constraints : ASCII, [1-120] characters, unqiue in file and
   *                     submitter.
   * 
   * Notes: Provide a unique title that describes this Sample. We 
   * suggest that you use the system 
   * [biomaterial]-[condition(s)]-[replicate number], e.g., 
   * Muscle_exercised_60min_rep2.
   ********************************************************************/
int parse_Sample_title(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_supplementary_file
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Name of supplementary file, or 'none'.  Examples of 
   * supplementary file types include original Affymetrix CEL and EXP 
   * files, GenePix GPR files, and TIFF image files. Supplementary files 
   * should be zipped or tarred together with the SOFT file at time of 
   * submission (do not include any sub-directories or sub-folders in 
   * your zip/tar archive). Provision of supplementary raw data files 
   * facilitates the unambiguous interpretation of data and potential 
   * verification of conclusions as set forth in the MIAME guidelines.
   ********************************************************************/
int parse_Sample_supplementary_file(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_table
   * Number per file   : [0, 1]
   * Value constraints : ASCII, Tab-delimited table file , see notes.
   * 
   * Notes: Name of external CHP or tab-delimited file to be used as 
   * data table Affymetrix CHP file name:  If your processed data are 
   * CHP files, you can reference the CHP file name in this field. If 
   * your manuscript discusses data processed by RMA or another 
   * algorithm, we recommend providing those values in the table 
   * section. There is no need to specify the !Sample_platform_id when 
   * CHP files are supplied. All external files should be zipped or 
   * tarred together with the SOFT file at time of submission.
   * 
   * 
   * If it is convenient for you to generate, you can reference the name 
   * of an external tab-delimited table file (see format) in this field, 
   * rather than include the table in the !Sample_table_begin section. 
   * All external files should be zipped or tarred together with the 
   * SOFT file at time of submission.
   ********************************************************************/
int parse_Sample_table(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_source_name_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII
   * 
   * Notes: Briefly identify the biological material and the 
   * experimental variable(s), e.g., vastus lateralis muscle, exercised, 
   * 60 min.
   ********************************************************************/
int parse_Sample_source_name_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_organism_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Use standard NCBI Taxonomy nomenclature Identify the 
   * organism(s) from which the biological material was derived.
   ********************************************************************/
int parse_Sample_organism_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_characteristics_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, 'Tag: Value' format
   * 
   * Notes: Describe all available characteristics of the biological 
   * source, including factors not necessarily under investigation. 
   * Provide in 'Tag: Value' format, where 'Tag' is a type of 
   * characteristic (e.g. "gender", "strain", "tissue", "developmental 
   * stage", "tumor stage", etc), and 'Value' is the value for each tag 
   * (e.g. "female", "129SV", "brain", "embryo", etc). Include as many 
   * characteristics fields as necessary to thoroughly describe your 
   * Samples.
   ********************************************************************/
int parse_Sample_characteristics_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_biomaterial_provider_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Specify the name of the company, laboratory or person that 
   * provided the biological material.
   ********************************************************************/
int parse_Sample_biomaterial_provider_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_treatment_protocol_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe any treatments applied to the biological material 
   * prior to extract preparation. You can include as much text as you 
   * need to thoroughly describe the protocol; it is strongly 
   * recommended that complete protocol descriptions are provided within 
   * your submission.
   ********************************************************************/
int parse_Sample_treatment_protocol_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_growth_protocol_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe the conditions that were used to grow or maintain 
   * organisms or cells prior to extract preparation. You can include as 
   * much text as you need to thoroughly describe the protocol; it is 
   * strongly recommended that complete protocol descriptions are 
   * provided within your submission.
   ********************************************************************/
int parse_Sample_growth_protocol_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_molecule_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII, Specify the type of molecule that was 
   *                     extracted from the biological material.
   * 
   * Notes:
   ********************************************************************/
int parse_Sample_molecule_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_extract_protocol_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe the protocol used to isolate the extract material. 
   * You can include as much text as you need to thoroughly describe the 
   * protocol; it is strongly recommended that complete protocol 
   * descriptions are provided within your submission.
   ********************************************************************/
int parse_Sample_extract_protocol_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_label_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII, Specify the compound used to label the 
   *                     extract e.g., biotin, Cy3, Cy5, 33P.
   * 
   * Notes:
   ********************************************************************/
int parse_Sample_label_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_label_protocol_ch[n]
   * Number per file   : [1, Infinty)
   * Value constraints : ASCII, extraction protocol description.
   * 
   * Notes: Describe the protocol used to label the extract. You can 
   * include as much text as you need to thoroughly describe the 
   * protocol; it is strongly recommended that complete protocol 
   * descriptions are provided within your submission.
   ********************************************************************/
int parse_Sample_label_protocol_ch(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_hyb_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, describe hybridization protocol used.
   * 
   * Notes: Describe the protocols used for hybridization, blocking and 
   * washing, and any post-processing steps such as staining. You can 
   * include as much text as you need to thoroughly describe the 
   * protocol; it is strongly recommended that complete protocol 
   * descriptions are provided within your submission.
   ********************************************************************/
int parse_Sample_hyb_protocol(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_scan_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, scanning/image acquisition protocols.
   * 
   * Notes: Describe the scanning and image acquisition protocols, 
   * hardware, and software. You can include as much text as you need to 
   * thoroughly describe the protocol; it is strongly recommended that 
   * complete protocol descriptions are provided within your submission.
   ********************************************************************/
int parse_Sample_scan_protocol(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_data_processing
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Provide details of how data in the VALUE column of your 
   * table were generated and calculated, i.e., normalization method, 
   * data selection procedures and parameters, transformation algorithm 
   * (e.g., MAS5.0), and scaling parameters. You can include as much 
   * text as you need to thoroughly describe the processing procedures.
   ********************************************************************/
int parse_Sample_data_processing(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_description
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Include any additional information not provided in the other 
   * fields, or paste in broad descriptions that cannot be easily 
   * dissected into the other fields.
   ********************************************************************/
int parse_Sample_description(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_platform_id
   * Number per file   : 1
   * Value constraints : ASCII, a valid Platform identifier.
   * 
   * Notes: Reference the Platform upon which this hybridization was 
   * performed.  Reference the Platform accession number (GPLxxx) if the 
   * Platform already exists in GEO, or reference the ^Platform 
   * identifier if the Platform record is being batch submitted within 
   * the same SOFT file.  To identify the accession number of an 
   * existing commercial Platform in GEO, use the FIND PLATFORM tool.
   ********************************************************************/
int parse_Sample_platform_id(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Integer, valid Sample accession number.
   * 
   * Notes: a valid Sample accession number (GSMxxx).  Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
int parse_Sample_geo_accession(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_anchor
   * Number per file   : 1
   * Value constraints : See notes.
   * 
   * Notes: SAGE enzyme anchor, usually NlaIII or Sau3A   Use for SAGE 
   * submissions only.
   ********************************************************************/
int parse_Sample_anchor(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_type
   * Number per file   : 1
   * Value constraints : ASCII, SAGE.
   * 
   * Notes: SAGE   Use for SAGE submissions only.
   ********************************************************************/
int parse_Sample_type(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_tag_count
   * Number per file   : 1
   * Value constraints : Number, sum of tags quantified in SAGE library.
   * 
   * Notes: Use for SAGE submissions only.
   ********************************************************************/
int parse_Sample_tag_count(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_tag_length
   * Number per file   : 1
   * Value constraints : Interger, base pair length of the SAGE tags, 
   *                     excluding anchor sequence.
   * 
   * Notes: Use for SAGE submissions only.
   ********************************************************************/
int parse_Sample_tag_length(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : ^SERIES
   * Number per file   : 1
   * Value constraints : ASCII, unique in file.
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records.
   ********************************************************************/
int parse_SERIES(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_title
   * Number per file   : 1
   * Value constraints : ASCII, [1, 255] characters, unique in file and 
   *                     by submitter.
   * 
   * Notes: Provide a unique title that describes the overall study.
   ********************************************************************/
int parse_Series_title(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_summary
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Summarize the goals and objectives of this study. The 
   * abstract from the associated publication may be suitable. You can 
   * include as much text as you need to thoroughly describe the study.
   ********************************************************************/
int parse_Series_summary(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_overall_design
   * Number per file   : 1
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Provide a description of the experimental design.  Indicate 
   * how many Samples are analyzed, if replicates are included, are 
   * there control and/or reference Samples, dye-swaps, etc.
   ********************************************************************/
int parse_Series_overall_design(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_pubmed_id
   * Number per file   : [0, Infinity)
   * Value constraints : PubMed identifier
   * 
   * Notes: Specify a valid PubMed identifier (PMID) that references a 
   * published article describing this study. Most commonly, this 
   * information is not available at the time of submission - it can be 
   * added later once the data are published.
   ********************************************************************/
int parse_Series_pubmed_id(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_web_link
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, valid URL.
   * 
   * Notes: Specify a Web link that directs users to supplementary 
   * information about the study. Please restrict to Web sites that you 
   * know are stable.
   ********************************************************************/
int parse_Series_web_link(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_contributor
   * Number per file   : [0, Infinity)
   * Value constraints : Contributor names, see notes.
   * 
   * Notes: List all people associated with this study.  Each value in 
   * the form 'firstname,middleinitial,lastname' or 
   * 'firstname,lastname': firstname must be at least one character and 
   * cannot contain spaces; middleinitial, if present, is one character; 
   * lastname is at least two characters and can contain spaces.   
   ********************************************************************/
int parse_Series_contributor(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_variable_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe dose, time, tissue, strain, gender, cell line, 
   * development stage, age, agent, cell type, infection, isolate, 
   * metabolism, shock, stress, temperature, specimen, disease state, 
   * protocol, growth protocol, genotype/variation, species, individual, 
   * or other.  
   * Indicate the variable type(s) investigated in this study, e.g., 
   * !Series_variable_1 = age
   * !Series_variable_2 = age
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
int parse_Series_variable(const char *toParse, struct GEOSimpleFile *toParse);
   
  /* Label/Key         : !Series_variable_description_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Describe each variable, e.g., 
   * !Series_variable_description_1 = 2 months
   * !Series_variable_description_2 = 12 months  
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
int parse_Series_variable_description(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_variable_sample_list_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : 
   * 
   * Notes: Each value a valid reference to a ^SAMPLE identifier, or all   
   * List which Samples belong to each group, e.g.,
   * !Series_variable_sample_list_1 = samA, samB
   * !Series_variable_sample_list_2 = samC, samD 
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
int parse_Series_variable_sample_list(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_repeats_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: biological replicate, technical replicate - extract, or 
   * technical replicate - labeled-extract   Indicate the repeat 
   * type(s), e.g.,
   * !Series_repeats_1 = biological replicate
   * !Series_repeats_2 = biological replicate 
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
int parse_Series_repeats(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_repeats_sample_list_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Each value a valid reference to a ^SAMPLE identifier, or all   
   * List which Samples belong to each group, e.g.,
   * !Series_repeats_sample_list_1 = samA, samB
   * !Series_repeats_sample_list_2 = samC, samD 
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
int parse_Series_repeats_sample_list(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_sample_id
   * Number per file   : [1, Infinity)
   * Value constraints : Valid Sample identifiers
   * 
   * Notes: Reference the Samples that make up this experiment. 
   * Reference the Sample accession numbers (GSMxxx) if the Samples 
   * already exists in GEO, or reference the ^Sample identifiers if they 
   * are being submitted in the same file.
   ********************************************************************/
int parse_Series_sample_id(const char* toParse, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Number
   * 
   * Notes: A valid Series accession number (GSExxx)   Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
int parse_Series_geo_accession(const char* toParse, struct GEOSimpleFile *parseTo);



static const  unordered_map<std::string, int (*func)(const char*, struct GEOSimpleFile*)> platformParseLookup = {
  {"^PLATFORM",                       parse_PLATFORM},
  {"!Platform_title",                 parse_Platform_title},
  {"!Platform_distribution",          parse_Platform_distribution},
  {"!Platform_technology",            parse_Platform_technology},
  {"!Platform_organism",              parse_Platform_organism},
  {"!Platform_manufacturer",          parse_Platform_manufacturer},
  {"!Platform_manufacture_protocol",  parse_Platform_manufacture_protocol},
  {"!Platform_catalog_number",        parse_Platform_catalog_number},
  {"!Platform_web_link",              parse_Platform_web_link},
  {"!Platform_support",               parse_Platform_support},
  {"!Platform_coating",               parse_Platform_coating},
  {"!Platform_description",           parse_Platform_description},
  {"!Platform_contributor",           parse_Platform_contributor},
  {"!Platform_pubmed_id",             parse_Platform_pubmed_id},
  {"!Platform_geo_accession",         parse_Platform_geo_accession},
  {"^SAMPLE",                         parse_SAMPLE},
  {"!Sample_title",                   parse_Sample_title},
  {"!Sample_supplementary_file",      parse_Sample_supplementary_file},
  {"!Sample_table",                   parse_Sample_table},
  {"!Sample_source_name_ch",          parse_Sample_source_name_ch},
  {"!Sample_organism_ch",             parse_Sample_organism_ch},
  {"!Sample_characteristics_ch",      parse_Sample_characteristics_ch},
  {"!Sample_biomaterial_provider_ch", parse_Sample_biomaterial_provider_ch},
  {"!Sample_treatment_protocol_ch",   parse_Sample_treatment_protocol_ch},
  {"!Sample_growth_protocol_ch",      parse_Sample_growth_protocol_ch},
  {"!Sample_molecule_ch",             parse_Sample_molecule_ch},
  {"!Sample_extract_protocol_ch",     parse_Sample_extract_protocol_ch},
  {"!Sample_label_ch",                parse_Sample_label_ch},
  {"!Sample_label_protocol_ch",       parse_Sample_label_protocol_ch},
  {"!Sample_hyb_protocol",            parse_Sample_hyb_protocol},
  {"!Sample_scan_protocol",           parse_Sample_scan_protocol},
  {"!Sample_data_processing",         parse_Sample_data_processing},
  {"!Sample_description",             parse_Sample_description},
  {"!Sample_platform_id",             parse_Sample_platform_id},
  {"!Sample_geo_accession",           parse_Sample_geo_accession},
  {"!Sample_anchor",                  parse_Sample_anchor},
  {"!Sample_type",                    parse_Sample_type},
  {"!Sample_tag_count",               parse_Sample_tag_count},
  {"!Sample_tag_length",              parse_Sample_tag_length},
  {"^SERIES",                         parse_SERIES},
  {"!Series_title",                   parse_Series_title},
  {"!Series_summary",                 parse_Series_summary},
  {"!Series_overall_design",          parse_Series_overall_design},
  {"!Series_pubmed_id",               parse_Series_pubmed_id},
  {"!Series_web_link",                parse_Series_web_link},
  {"!Series_contributor",             parse_Series_contributor},
  {"!Series_variable",                parse_Series_variable},
  {"!Series_variable_description",    parse_Series_variable_description},
  {"!Series_variable_sample_list",    parse_Series_variable_sample_list},
  {"!Series_repeats",                 parse_Series_repeats},
  {"!Series_repeats_sample_list",     parse_Series_repeats_sample_list},
  {"!Series_sample_id",               parse_Series_sample_id},
  {"!Series_geo_accession",           parse_Series_geo_accession},
  {"!Platform_table_begin",           NULL},
  {"!Sample_table_begin",             NULL} };


int aceptableASCIIString(const char *str){
  if(NULL == str) return 0;
  while(1){
    switch(*str){
      case 0   : return 1; //end parse
      case 10  : 
      case 12  : 
      case 13  : 
      case 32  : 
      case 33  : 
      case 34  : 
      case 35  : 
      case 36  : 
      case 37  : 
      case 38  : 
      case 39  : 
      case 40  : 
      case 41  : 
      case 42  : 
      case 43  : 
      case 44  : 
      case 45  : 
      case 46  : 
      case 47  : 
      case 48  : 
      case 49  : 
      case 50  : 
      case 51  : 
      case 52  : 
      case 53  : 
      case 54  : 
      case 55  : 
      case 56  : 
      case 57  : 
      case 58  : 
      case 59  : 
      case 60  : 
      case 61  : 
      case 62  : 
      case 63  : 
      case 64  : 
      case 65  : 
      case 66  : 
      case 67  : 
      case 68  : 
      case 69  : 
      case 70  : 
      case 71  : 
      case 72  : 
      case 73  : 
      case 74  : 
      case 75  : 
      case 76  : 
      case 77  : 
      case 78  : 
      case 79  : 
      case 80  : 
      case 81  : 
      case 82  : 
      case 83  : 
      case 84  : 
      case 85  : 
      case 86  : 
      case 87  : 
      case 88  : 
      case 89  : 
      case 90  : 
      case 91  : 
      case 92  : 
      case 93  : 
      case 94  : 
      case 95  : 
      case 96  : 
      case 97  : 
      case 98  : 
      case 99  : 
      case 100 : 
      case 101 : 
      case 102 : 
      case 103 : 
      case 104 : 
      case 105 : 
      case 106 : 
      case 107 : 
      case 108 : 
      case 109 : 
      case 110 : 
      case 111 : 
      case 112 : 
      case 113 : 
      case 114 : 
      case 115 : 
      case 116 : 
      case 117 : 
      case 118 : 
      case 119 : 
      case 120 : 
      case 121 : 
      case 122 : 
      case 123 : 
      case 124 : 
      case 125 : 
      case 126 : str++; break;
      default  : return 0;
  }
  return 1;
}


std::pair<std::string, std::string> getKeyValuePair(const char *line){
  std::pair<std::string, std::string> toReturn;
  toReturn = std::pair<std::string, std::string>(std::string(""), std::string(""));
  
  if(!aceptableASCIIString(line)) return toReturn;
  
  if(!strlen(line)) return toReturn;
  
  char *linePtr = strdup(line);
  char *key = strtok(line, "=");
  
  if(strlen(key) < 2){
    free(linePtr);
    return toReturn;
  }
  
  char *value = key+2;
  
  while(isspace(*key)) key++;
  while(isspace(*value)) value++;
  
  char *endPtr = key + strlen(key) - 1;
  while(endPtr > str && isspace(*endPtr)) endPtr--;
  *(endPtr+1) = 0;
  
  char *endPtr = value + strlen(value) - 1;
  while(endPtr > str && isspace(*endPtr)) endPtr--;
  *(endPtr+1) = 0;
  
  if(!strlen(key)){
    free(linePtr);
    return toReturn;
  }
  
  if(!strlen(value)){
    free(linePtr);
    return toReturn;
  }
  
  toReturn = std::pair<std::string, std::string>(std::string(key), std::string(value));
  
  free(linePtr);
   
  return toReturn;
}


  /* Label/Key         : ^PLATFORM
   * Number per file   : 1
   * Value constraints : Unique key, ASCII
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records. 
   ********************************************************************/
int valid_PLATFORM(const std::string value, struct GEOSimpleFile *parseTo){
  if(NULL != parseTo->PLATFORM) return -EINVAL;
  if(!value.size()) return -EINVAL;

  parseTo->PLATFORM = strdup(value.c_str());

  return 0;
}


  /* Label/Key         : !Platform_title
   * Number per file   : 1
   * Value constraints : string of length 1-120 characters, must be 
   * unique within local file and over all previously submitted 
   * Platforms for that submitter.
   ********************************************************************/
int valid_Platform_title(const std::string value, struct GEOSimpleFile *parseTo){
  if(NULL != parseTo->Platform_title) return -EINVAL;
  if(!value.length()) return -EINVAL;
  
  if(value.size() > 120)  return -EINVAL;
  
  parseTo->Platform_title = strdup(value.c_str());
  
  return 0;
}


  /* Label/Key         : !Platform_distribution
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, 
   *                     custom-commercial}
   ********************************************************************/
int valid_Platform_distribution(const char *line, struct GEOSimpleFile *parseTo){
  if(NULL != parseTo->Platform_distribution) return -EINVAL;
  
}


  /* Label/Key         : !Platform_technology
   * Number per file   : 1
   * Value constraints : in {spotted DNA/cDNA, spotted oligonucleotide, 
   *                     in situ oligonucleotide, antibody, tissue, 
   *                     SARST, RT-PCR, or MPSS}
   ********************************************************************/
int valid_Platform_technology(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_organism
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Platform_organism(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_manufacturer
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
int valid_Platform_manufacturer(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_manufacture_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Platform_manufacture_protocol(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_catalog_number
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Platform_catalog_number(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_web_link
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, valid URL
   ********************************************************************/
int valid_Platform_web_link(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_support
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   ********************************************************************/
int valid_Platform_support(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_coating
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   * 
   * Notes: Provide the coating of the array, e.g., aminosilane, quartz, 
   * polysine, unknown.
   ********************************************************************/
int valid_Platform_coating(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_description
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Platform_description(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_contributor
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, List all people associated with this 
   *                     array design.
   * 
   * Notes: Each value in the form 'firstname,middleinitial,lastname' or 
   * 'firstname,lastname': firstname must be at least one character and 
   * cannot contain spaces; middleinitial, if present, is one character; 
   * lastname is at least two characters and can contain spaces. 
   ********************************************************************/
int valid_Platform_contributor(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_pubmed_id
   * Number per file   : [0, Infinity)
   * Value constraints : Integer, valid PubMed identifier (PMID)
   ********************************************************************/
int valid_Platform_pubmed_id(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Platform_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Integer, valid Platform accession number 
   *                     (GPLxxx)
   ********************************************************************/
int valid_Platform_geo_accession(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : ^SAMPLE
   * Number per file   : 1
   * Value constraints : ASCII, Unique within file
   ********************************************************************/
int valid_SAMPLE(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_title
   * Number per file   : 1
   * Value constraints : ASCII, [1-120] characters, unqiue in file and
   *                     submitter.
   ********************************************************************/
int valid_Sample_title(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_supplementary_file
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_supplementary_file(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_table
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_table(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_source_name_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_source_name_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_organism_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * TODO: extensive verification -- way too hard and time consuming for
   * v1.
   ********************************************************************/
int valid_Sample_organism_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_characteristics_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, "STRING1: STRING2" tag-value format
   ********************************************************************/
int valid_Sample_characteristics_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_biomaterial_provider_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_biomaterial_provider_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_treatment_protocol_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe any treatments applied to the biological material 
   * prior to extract preparation. You can include as much text as you 
   * need to thoroughly describe the protocol; it is strongly 
   * recommended that complete protocol descriptions are provided within 
   * your submission.
   ********************************************************************/
int valid_Sample_treatment_protocol_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_growth_protocol_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_growth_protocol_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_molecule_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_molecule_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_extract_protocol_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_extract_protocol_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_label_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_label_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_label_protocol_ch[n]
   * Number per file   : [1, Infinty)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_label_protocol_ch(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_hyb_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_hyb_protocol(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_scan_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_scan_protocol(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_data_processing
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_data_processing(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_description
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_description(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_platform_id
   * Number per file   : 1
   * Value constraints : ASCII, (GPLxxx) if the Platform already 
   *                     exists in GEO, else ^Platform
   ********************************************************************/
int valid_Sample_platform_id(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : ASCII, valid Sample accession number; (GSMxxx)
   ********************************************************************/
int valid_Sample_geo_accession(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_anchor
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_anchor(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_type
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
int valid_Sample_type(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_tag_count
   * Number per file   : 1
   * Value constraints : Number
   ********************************************************************/
int valid_Sample_tag_count(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Sample_tag_length
   * Number per file   : 1
   * Value constraints : Interger
   ********************************************************************/
int valid_Sample_tag_length(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : ^SERIES
   * Number per file   : 1
   * Value constraints : ASCII, unique
   ********************************************************************/
int valid_SERIES(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_title
   * Number per file   : 1
   * Value constraints : ASCII, [1, 255] characters, unique
   ********************************************************************/
int valid_Series_title(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_summary
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Series_summary(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_overall_design
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
int valid_Series_overall_design(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_pubmed_id
   * Number per file   : [0, Infinity)
   * Value constraints : PubMed identifier
   ********************************************************************/
int valid_Series_pubmed_id(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_web_link
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, valid URL
   ********************************************************************/
int valid_Series_web_link(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_contributor
   * Number per file   : [0, Infinity)
   * Value constraints : Contributor names, see notes.
   * 
   * Notes: List all people associated with this study.  Each value in 
   * the form 'firstname,middleinitial,lastname' or 
   * 'firstname,lastname': firstname must be at least one character and 
   * cannot contain spaces; middleinitial, if present, is one character; 
   * lastname is at least two characters and can contain spaces.   
   ********************************************************************/
int valid_Series_contributor(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_variable_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Series_variable(const char *line, struct GEOSimpleFile *parseTo);
   
  /* Label/Key         : !Series_variable_description_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Series_variable_description(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_variable_sample_list_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : 
   * 
   * Notes: Each value a valid reference to a ^SAMPLE identifier, or all   
   * List which Samples belong to each group, e.g.,
   * !Series_variable_sample_list_1 = samA, samB
   * !Series_variable_sample_list_2 = samC, samD 
   ********************************************************************/
int valid_Series_variable_sample_list(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_repeats_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   ********************************************************************/
int valid_Series_repeats(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_repeats_sample_list_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Each value a valid reference to a ^SAMPLE identifier, or all   
   * List which Samples belong to each group, e.g.,
   * !Series_repeats_sample_list_1 = samA, samB
   * !Series_repeats_sample_list_2 = samC, samD 
   ********************************************************************/
int valid_Series_repeats_sample_list(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_sample_id
   * Number per file   : [1, Infinity)
   * Value constraints : Valid Sample identifiers
   * 
   * Notes: Reference the Samples that make up this experiment. 
   * Reference the Sample accession numbers (GSMxxx) if the Samples 
   * already exists in GEO, or reference the ^Sample identifiers if they 
   * are being submitted in the same file.
   ********************************************************************/
int valid_Series_sample_id(const char *line, struct GEOSimpleFile *parseTo);

  /* Label/Key         : !Series_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Number
   * 
   * Notes: A valid Series accession number (GSExxx)   Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
int valid_Series_geo_accession(const char *line, struct GEOSimpleFile *parseTo);





enum platformHeaderTypes {ID, SEQUENCE, GB_ACC, GB_LIST, GB_RANGE, 
  RANGE_GB, RANGE_START, RANGE_END, RANGE_STRAND, GI, GI_LIST, GI_RANGE, 
  CLONE_ID, CLONE_ID_LIST, ORF, ORF_LIST, GENOME_ACC, SNP_ID, 
  SNP_ID_LIST, miRNA_ID, miRNA_ID_LIST, SPOT_ID, ORGANISM, PT_ACC, 
  PT_LIST, PT_GI, PT_GI_LIST, SP_ACC, SP_LIST};
/*******************************************************************//**
 * Column Label: ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SEQUENCE
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GB_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GB_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GB_RANGE
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_GB
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_START
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_END
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_STRAND
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GI
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GI_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GI_RANGE
 * Data Type   : 
 * Description : 
 * 
 * Column Label: CLONE_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: CLONE_ID_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: ORF
 * Data Type   : 
 * Description : 
 * 
 * Column Label: ORF_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GENOME_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SNP_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SNP_ID_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: miRNA_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: miRNA_ID_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SPOT_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: ORGANISM
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_GI
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_GI_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SP_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SP_LIST
 * Data Type   : 
 * Description : 
 * 
***********************************************************************/

struct platformTable{
  size_t numCols;
  enum platformHeaderTypes *headers;
  void **data;
};


enum sampleHeaderTypes{ID_REF, VALUE};
/*******************************************************************//**
 * Column Label: ID_REF
 * Data Type   : 
 * Description : 
 * 
 * Column Label: VALUE
 * Data Type   : 
 * Description : 
 * 
***********************************************************************/


struct sampleTable{
  size_t numCols;
  enum sampleHeaderTypes *headers;
  void **data
};


struct GEOSimpleFile{
  
  char* PLATFORM;
  
  char* Platform_title;
  
  char* Platform_distribution;

  char* Platform_technology;
  
  char** Platform_organism;
  
  char* Platform_manufacturer;
  
  char** Platform_manufacture_protocol;
  
  char** Platform_catalog_number;
  
  char** Platform_web_link;
  
  char* Platform_support;
  
  char* Platform_coating;
  
  char** Platform_description;
  
  char** Platform_contributor;
  
  char** Platform_pubmed_id;
  
  char* Platform_geo_accession;
  
  struct Platform_table platformTable;
  /* Label/Key         : !Platform_table_begin
   * Number per file   : 1
   * Value constraints : N/A, See notes.
   * 
   * Notes: No content required, indicates the start of the data table.
   * Until !Platform_table_end is reached, a tab seperated va;ue table
   * is described.
   ********************************************************************/
  /* Label/Key         : !Platform_table_end
   * Number per file   : 1
   * Value constraints : N/A, See notes.
   * 
   * Notes: Ends table section opened by !Platform_table_begin.
   ********************************************************************/
  
  //////////////////////////////////////////////////////////////////////
  
  char* SAMPLE;
   
  char* Sample_title;
   
  char** Sample_supplementary_file;
   
  char* Sample_table;
  
  char** Sample_source_name_ch;
   
  char** Sample_organism_ch;
   
  char** Sample_characteristics_ch;
   
  char** Sample_biomaterial_provider_ch;
  
  char** Sample_treatment_protocol;
   
  char** Sample_growth_protocol_ch;
   
  char** Sample_molecule_ch;
   
  char** Sample_extract_protocol_ch;
   
  char** Sample_label_ch;
  
  char** Sample_label_protocol_ch;
   
  char** Sample_hyb_protocol;
   
  char** Sample_scan_protocol;
   
  char** Sample_data_processing;
   
  char** Sample_description;
   
  char* Sample_platform_id;
   
  char* Sample_geo_accession;
   
  char* Sample_anchor;
   
  char* Sample_type;
  
  char* Sample_tag_count;
   
  char* Sample_tag_length;
   
   //TODO: add doc, detail
   struct Sample_table sampleTable;
  /* Label/Key         : !Sample_table_begin
   * Number per file   : 
   * Value constraints : 
   * 
   * Notes:
   ********************************************************************/
  /* Label/Key         : !Sample_table_begin
   * Number per file   : 
   * Value constraints : 
   * 
   * Notes:
   ********************************************************************/
  
  //////////////////////////////////////////////////////////////////////
  
  char* SERIES;
   
  char* Series_title;
   
  char** Series_summary;
   
  char* Series_overall_design;
   
  char** Series_pubmed_id;
   
  char** Series_web_link;
   
  char** Series_contributor;
   
  char** Series_variable;
   
  char** Series_variable_description;
  
  char** Series_variable_sample_list;
  
  //TODO: fix up doc
  char** Series_repeats;
  
  //TODO: fix up doc
  char** Series_repeats_sample_list;
  
  char** Series_sample_id;
   
  char* Series_geo_accession;
  
};



int verifyGEOSimpleString(const simpleFile *toVerify){
  if(!valid_PLATFORM()){
    
  }
  
  Platform_title;
  
  Platform_distribution;

  Platform_technology;
  
  Platform_organism;
  
  Platform_manufacturer;
  
  Platform_manufacture_protocol;
  
  Platform_catalog_number;
  
  Platform_web_link;
  
  Platform_support;
  
  Platform_coating;
  
  Platform_description;
  
  Platform_contributor;
  
  Platform_pubmed_id;
  
  Platform_geo_accession;
  
  struct Platform_table platformTable;
  /* Label/Key         : !Platform_table_begin
   * Number per file   : 1
   * Value constraints : N/A, See notes.
   * 
   * Notes: No content required, indicates the start of the data table.
   * Until !Platform_table_end is reached, a tab seperated va;ue table
   * is described.
   ********************************************************************/
  /* Label/Key         : !Platform_table_end
   * Number per file   : 1
   * Value constraints : N/A, See notes.
   * 
   * Notes: Ends table section opened by !Platform_table_begin.
   ********************************************************************/
  
  //////////////////////////////////////////////////////////////////////
  
  SAMPLE;
   
  Sample_title;
   
  Sample_supplementary_file;
   
  Sample_table;
  
  Sample_source_name_ch;
   
  Sample_organism_ch;
   
  Sample_characteristics_ch;
   
  Sample_biomaterial_provider_ch;
  
  Sample_treatment_protocol;
   
  Sample_growth_protocol_ch;
   
  Sample_molecule_ch;
   
  Sample_extract_protocol_ch;
   
  Sample_label_ch;
  
  Sample_label_protocol_ch;
   
  Sample_hyb_protocol;
   
  Sample_scan_protocol;
   
  Sample_data_processing;
   
  Sample_description;
   
  Sample_platform_id;
   
  Sample_geo_accession;
   
  Sample_anchor;
   
  Sample_type;
  
  Sample_tag_count;
   
  Sample_tag_length;
   
  struct Sample_table sampleTable;
  
  SERIES;
   
  Series_title;
   
  Series_summary;
   
  Series_overall_design;
   
  Series_pubmed_id;
   
  Series_web_link;
   
  Series_contributor;
   
  Series_variable;
   
  Series_variable_description;
  
  Series_variable_sample_list;
  
  Series_repeats;
  
  Series_repeats_sample_list;
  
  Series_sample_id;
   
  Series_geo_accession;
}

