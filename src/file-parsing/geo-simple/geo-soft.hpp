#ifndef GEO_SOFT_HPP
#define GEO_SOFT_HPP


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any, must be unique within local file

Content guidelines: Provide an identifier for this entity. This identifier is
used only as an internal reference within a given file. The identifier will not
appear on final GEO records.
*******************************************************************************/
#define CSTRING_PLATFORM "^PLATFORM"
//std::string platform;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: string of length 1-120 characters, must be
unique within local file and over all previously submitted Platforms for that
submitter

Content guidelines: Provide a unique title that describes your Platform. We
suggest that you use the system
[institution/lab]-[species]-[number of features]-[version], e.g.
"FHCRC Mouse 15K v1.0".
*******************************************************************************/
#define CSTRING_Platform_title "!Platform_title"
//std::string platform_title;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: commercial, non-commercial, custom-commercial,
or virtual

Content guidelines: Microarrays are 'commercial', 'non-commercial', or
'custom-commercial' in accordance with how the array was manufactured. Use
'virtual' only if creating a virtual definition for MPSS, SARST, or RT-PCR data.
*******************************************************************************/
#define CSTRING_Platform_distribution "!Platform_distribution"
enum platform_distribution_enum{
  commercial, non_commercial, custom_commercial, _virtual,
  platform_distribution_unset
};


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: spotted DNA/cDNA, spotted oligonucleotide,
in situ oligonucleotide, antibody, tissue, SARST, RT-PCR, or MPSS

Content guidelines: Select the category that best describes the Platform
technology.
*******************************************************************************/
#define CSTRING_Platform_technology "!Platform_technology"
enum platform_technology_enum{
  spotted_DNA_or_cDNA, spotted_oligonucleotide, in_situ_oligonucleotide,
  antibody, tissue, SARST, RT_PCR, MPSS, platform_technology_unset
};


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: use standard NCBI Taxonomy nomenclature

Content guidelines: Identify the organism(s) from which the features on the
Platform were designed or derived.
*******************************************************************************/
#define CSTRING_Platform_organism "!Platform_organism"
//std::vector<std::string> platform_organism;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any

Content guidelines: Provide the name of the company, facility or laboratory
where the array was manufactured or produced.
*******************************************************************************/
#define CSTRING_Platform_manufacturer "!Platform_manufacturer"
//std::string platform_manufacturer;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the array manufacture protocol. Include as much
detail as possible, e.g., clone/primer set identification and preparation,
strandedness/length, arrayer hardware/software, spotting protocols. You can
include as much text as you need to thoroughly describe the protocol; it is
strongly recommended that complete protocol descriptions are provided within
your submission.
*******************************************************************************/
#define CSTRING_Platform_manufacture_protocol "!Platform_manufacture_protocol"
//std::vector<std::string> platform_manufacture_protocol;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Provide the manufacturer catalog number for commercially-
available arrays.
*******************************************************************************/
#define CSTRING_Platform_catalog_number "!Platform_catalog_number"
//std::vector<std::string> platform_catalog_number;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: valid URL

Content guidelines: Specify a Web link that directs users to supplementary
information about the array. Please restrict to Web sites that you know are
stable.
*******************************************************************************/
#define CSTRING_Platform_web_link "!Platform_web_link"
//std::vector<std::string> platform_web_link;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: any

Content guidelines: Provide the surface type of the array, e.g., glass,
nitrocellulose, nylon, silicon, unknown.
*******************************************************************************/
#define CSTRING_Platform_support "!Platform_support"
//std::string platform_support;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: any

Content guidelines: Provide the coating of the array, e.g., aminosilane, quartz,
polysine, unknown.
*******************************************************************************/
#define CSTRING_Platform_coating "!Platform_coating"
//std::string platform_coating;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Provide any additional descriptive information not captured
in another field, e.g., array and/or feature physical dimensions, element grid
system.
*******************************************************************************/
#define CSTRING_Platform_description "!Platform_description"
//std::string platform_description;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: each value in the form
'firstname,middleinitial,lastname' or 'firstname,lastname': firstname must be at
least one character and cannot contain spaces; middleinitial, if present, is one
character; lastname is at least two characters and can contain spaces.

Content guidelines: List all people associated with this array design.
*******************************************************************************/
#define CSTRING_Platform_contributor "!Platform_contributor"
//std::vector<std::string> platform_contributor;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: an integer

Content guidelines: Specify a valid PubMed identifier (PMID) that references a
published article that describes the array.
*******************************************************************************/
#define CSTRING_Platform_pubmed_id "!Platform_pubmed_id"
//std::vector<int> platform_pubmed_id;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: a valid Platform accession number (GPLxxx)

Content guidelines: Only use for performing updates to existing GEO records.
*******************************************************************************/
#define CSTRING_Platform_geo_accession "!Platform_geo_accession"
//std::string platform_geo_accession;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the start of the data table.
*******************************************************************************/
#define CSTRING_Platform_table_begin "!Platform_table_begin"
//std::vector<std::string> platform_table_column_titles;
//std::vector<std::vector<std::string> > platform_table_matrix;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the end of the data table.
*******************************************************************************/
#define CSTRING_Platform_table_end "!Platform_table_end"


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any, must be unique within local file

Content guidelines: Provide an identifier for this entity. This identifier is
used only as an internal reference within a given file. The identifier will not
appear on final GEO records.
*******************************************************************************/
#define CSTRING_SAMPLE "^SAMPLE"
//std::string sample;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: string of length 1-120 characters, must be
unique within local file and over all previously submitted Samples for that
submitter

Content guidelines: Provide a unique title that describes this Sample. We
suggest that you use the system [biomaterial]-[condition(s)]-[replicate number],
e.g., Muscle_exercised_60min_rep2.
*******************************************************************************/
#define CSTRING_Sample_title "!Sample_title"
//std::string sample_title;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: name of supplementary file, or 'none'

Content guidelines: Examples of supplementary file types include original
Affymetrix CEL and EXP files, GenePix GPR files, and TIFF image files.
Supplementary files should be zipped or tarred together with the SOFT file at
time of submission (do not include any sub-directories or sub-folders in your
zip/tar archive). Provision of supplementary raw data files facilitates the
unambiguous interpretation of data and potential verification of conclusions as
set forth in the MIAME guidelines.
*******************************************************************************/
#define CSTRING_Sample_supplementary_file "!Sample_supplementary_file"
//std::vector<std::string> sample_supplementary_file;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: name of external CHP or tab-delimited file to be
used as data table

Content guidelines: - Affymetrix CHP file name:
If your processed data are CHP files, you can reference the CHP file name in
this field. If your manuscript discusses data processed by RMA or another
algorithm, we recommend providing those values in the table section. There is no
need to specify the !Sample_platform_id when CHP files are supplied. All
external files should be zipped or tarred together with the SOFT file at time of
submission.
- Tab-delimited table file name:
If it is convenient for you to generate, you can reference the name of an
external tab-delimited table file (see format) in this field, rather than
include the table in the !Sample_table_begin section. All external files should
be zipped or tarred together with the SOFT file at time of submission.
*******************************************************************************/
#define CSTRING_Sample_table "!Sample_table"
//std::string sample_table;


/*******************************************************************************
Number of allowed labels: 1 per channel

Allowed values and constraints: any

Content guidelines: Briefly identify the biological material and the
experimental variable(s), e.g., vastus lateralis muscle, exercised, 60 min.
*******************************************************************************/
#define CSTRING_Sample_source_name_ch "!Sample_source_name_ch"
//std::vector<std::string> sample_source_name;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: use standard NCBI Taxonomy nomenclature

Content guidelines: Identify the organism(s) from which the biological material
was derived.
*******************************************************************************/
#define CSTRING_Sample_organism_ch "!Sample_organism_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_organism;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: 'Tag: Value' format

Content guidelines: Describe all available characteristics of the biological
source, including factors not necessarily under investigation. Provide in
'Tag: Value' format, where 'Tag' is a type of characteristic (e.g. "gender",
"strain", "tissue", "developmental stage", "tumor stage", etc), and 'Value' is
the value for each tag (e.g. "female", "129SV", "brain", "embryo", etc). Include
as many characteristics fields as necessary to thoroughly describe your Samples.
*******************************************************************************/
#define CSTRING_Sample_characteristics_ch "!Sample_characteristics_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::pair<std::string, std::string> > >
//sample_characteristics;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Specify the name of the company, laboratory or person that
provided the biological material.
*******************************************************************************/
#define CSTRING_Sample_biomaterial_provider_ch "!Sample_biomaterial_provider_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_biomaterial_provider;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Describe any treatments applied to the biological material
prior to extract preparation. You can include as much text as you need to
thoroughly describe the protocol; it is strongly recommended that complete
protocol descriptions are provided within your submission.
*******************************************************************************/
#define CSTRING_Sample_treatment_protocol_ch "!Sample_treatment_protocol_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_treatment_protocol;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Describe the conditions that were used to grow or maintain
organisms or cells prior to extract preparation. You can include as much text as
you need to thoroughly describe the protocol; it is strongly recommended that
complete protocol descriptions are provided within your submission.
*******************************************************************************/
#define CSTRING_Sample_growth_protocol_ch "!Sample_growth_protocol_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_growth_protocol;


/*******************************************************************************
Number of allowed labels: 1 per channel

Allowed values and constraints: total RNA, polyA RNA, cytoplasmic RNA, nuclear
RNA, genomic DNA, protein, or other

Content guidelines: Specify the type of molecule that was extracted from the
biological material.
*******************************************************************************/
#define CSTRING_Sample_molecule_ch "!Sample_molecule_ch"
//std::vector<std::string> sample_molecule;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the protocol used to isolate the extract material.
You can include as much text as you need to thoroughly describe the protocol; it
is strongly recommended that complete protocol descriptions are provided within
your submission.
*******************************************************************************/
#define CSTRING_Sample_extract_protocol_ch "!Sample_extract_protocol_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_extract_protocol;


/*******************************************************************************
Number of allowed labels: 1 per channel

Allowed values and constraints: any

Content guidelines: Specify the compound used to label the extract e.g., biotin,
Cy3, Cy5, 33P.
*******************************************************************************/
#define CSTRING_Sample_label_ch "!Sample_label_ch"
//std::vector<std::string> sample_label;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the protocol used to label the extract. You can
include as much text as you need to thoroughly describe the protocol; it is
strongly recommended that complete protocol descriptions are provided within
your submission.
*******************************************************************************/
#define CSTRING_Sample_label_protocol_ch "!Sample_label_protocol_ch"
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_label_protocol;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the protocols used for hybridization, blocking and
washing, and any post-processing steps such as staining. You can include as much
text as you need to thoroughly describe the protocol; it is strongly recommended
that complete protocol descriptions are provided within your submission.
*******************************************************************************/
#define CSTRING_Sample_hyb_protocol "!Sample_hyb_protocol"
//std::vector<std::string> sample_hyb_protocol;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the scanning and image acquisition protocols,
hardware, and software. You can include as much text as you need to thoroughly
describe the protocol; it is strongly recommended that complete protocol
descriptions are provided within your submission.
*******************************************************************************/
#define CSTRING_Sample_scan_protocol "!Sample_scan_protocol"
//std::vector<std::string> sample_scan_protocol;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Provide details of how data in the VALUE column of your
table were generated and calculated, i.e., normalization method, data selection
procedures and parameters, transformation algorithm (e.g., MAS5.0), and scaling
parameters. You can include as much text as you need to thoroughly describe the
processing procedures.
*******************************************************************************/
#define CSTRING_Sample_data_processing "!Sample_data_processing"
//std::vector<std::string> sample_data_processing;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Include any additional information not provided in the other
fields, or paste in broad descriptions that cannot be easily dissected into the
other fields.
*******************************************************************************/
#define CSTRING_Sample_description "!Sample_description"
//std::vector<std::string> sample_description;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: a valid Platform identifier

Content guidelines: Reference the Platform upon which this hybridization was
performed. Reference the Platform accession number (GPLxxx) if the Platform
already exists in GEO, or reference the ^Platform identifier if the Platform
record is being batch submitted within the same SOFT file. To identify the
accession number of an existing commercial Platform in GEO, use the FIND
PLATFORM tool.
*******************************************************************************/
#define CSTRING_Sample_platform_id "!Sample_platform_id"
//std::string sample_platform_id;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: a valid Sample accession number (GSMxxx)

Content guidelines: Only use for performing updates to existing GEO records.
*******************************************************************************/
#define CSTRING_Sample_geo_accession "!Sample_geo_accession"
//std::string sample_geo_accession;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: SAGE enzyme anchor, usually NlaIII or Sau3A

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
#define CSTRING_Sample_anchor "!Sample_anchor"
//std::string sample_anchor;


/*******************************************************************************
Number of allowed labels:  1

Allowed values and constraints: SAGE

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
#define CSTRING_Sample_type "!Sample_type"
//std::string sample_type;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: sum of tags quantified in SAGE library

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
#define CSTRING_Sample_tag_count "!Sample_tag_count"
//int sample_tag_count;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: base pair length of the SAGE tags, excluding
anchor sequence

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
#define CSTRING_Sample_tag_length "!Sample_tag_length"
//int sample_tag_length;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the start of the data table.
*******************************************************************************/
#define CSTRING_Sample_table_begin "!Sample_table_begin"
//std::vector<std::string> sample_table_columns;
//std::vector<std::vector<std::string> > sample_table_matrix;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the end of the data table.
*******************************************************************************/
#define CSTRING_Sample_table_end "!Sample_table_end"


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any, must be unique within local file

Content guidelines: Provide an identifier for this entity. This identifier is
used only as an internal reference within a given file. The identifier will not
appear on final GEO records.
*******************************************************************************/
#define CSTRING_SERIES "^SERIES"
//std::string series;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: string of length 1-255 characters, must be
unique within local file and over all previously submitted Series for that
submitter

Content guidelines: string of length 1-255 characters, must be unique within
local file and over all previously submitted Series for that submitter
*******************************************************************************/
#define CSTRING_Series_title "!Series_title"
//std::string series_title;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Summarize the goals and objectives of this study. The
abstract from the associated publication may be suitable. You can include as
much text as you need to thoroughly describe the study.
*******************************************************************************/
#define CSTRING_Series_summary "!Series_summary"
//std::vector<std::string> series_summary;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any

Content guidelines: Provide a description of the experimental design. Indicate
how many Samples are analyzed, if replicates are included, are there control
and/or reference Samples, dye-swaps, etc.
*******************************************************************************/
#define CSTRING_Series_overall_design "!Series_overall_design"
//std::string series_overall_design;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: an integer

Content guidelines: Specify a valid PubMed identifier (PMID) that references a
published article describing this study. Most commonly, this information is not
available at the time of submission - it can be added later once the data are
published.
*******************************************************************************/
#define CSTRING_Series_pubmed_id "!Series_pubmed_id"
//std::vector<std::string> series_pubmed_id;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: valid URL

Content guidelines: Specify a Web link that directs users to supplementary
information about the study. Please restrict to Web sites that you know are
stable.
*******************************************************************************/
#define CSTRING_Series_web_link "!Series_web_link"
//std::vector<std::string> series_web_link;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: 'firstname,middleinitial,lastname' or
'firstname,lastname': firstname must be at least one character and cannot
contain spaces; middleinitial, if present, is one character; lastname is at
least two characters and can contain spaces.

Content guidelines: List all people associated with this study.
*******************************************************************************/
#define CSTRING_Series_contributor "!Series_contributor"
//std::vector<std::string> series_contributor;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: dose, time, tissue, strain, gender, cell line,
development stage, age, agent, cell type, infection, isolate, metabolism, shock,
stress, temperature, specimen, disease state, protocol, growth protocol,
genotype/variation, species, individual, or other

Content guidelines: Indicate the variable type(s) investigated in this study,
e.g.,
!Series_variable_1 = age
!Series_variable_2 = age
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
#define CSTRING_Series_variable_ "!Series_variable_"
//std::vector<std::string> series_variable;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Describe each variable, e.g.,
!Series_variable_description_1 = 2 months
!Series_variable_description_2 = 12 months
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
#define CSTRING_Series_variable_description_ "!Series_variable_description_"
//std::vector<std::string> series_variable_description;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: each value a valid reference to a ^SAMPLE
identifier, or all

Content guidelines: List which Samples belong to each group, e.g.,
!Series_variable_sample_list_1 = samA, samB
!Series_variable_sample_list_2 = samC, samD
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
#define CSTRING_Series_variable_sample_list_ "!Series_variable_sample_list_"
//std::vector<std::string> series_variable_sample_list;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: biological replicate, technical replicate -
extract, or technical replicate - labeled-extract

Content guidelines: Indicate the repeat type(s), e.g.,
!Series_repeats_1 = biological replicate
!Series_repeats_2 = biological replicate
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
#define CSTRING_Series_repeats_ "!Series_repeats_"
//std::vector<std::string> series_repeats;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: each value a valid reference to a ^SAMPLE
identifier, or all

Content guidelines:List which Samples belong to each group, e.g.,
!Series_repeats_sample_list_1 = samA, samB
!Series_repeats_sample_list_2 = samC, samD
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
#define CSTRING_Series_repeats_sample_list_ "!Series_repeats_sample_list_"
//std::vector<std::string> series_repeats_sample_list;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: valid Sample identifiers

Content guidelines: Reference the Samples that make up this experiment.
Reference the Sample accession numbers (GSMxxx) if the Samples already exists in
GEO, or reference the ^Sample identifiers if they are being submitted in the
same file.
*******************************************************************************/
#define CSTRING_Series_sample_id "!Series_sample_id"
//std::vector<std::string> series_sample_id;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: a valid Series accession number (GSExxx)

Content guidelines: Only use for performing updates to existing GEO records.
*******************************************************************************/
#define CSTRING_Series_geo_accession "!Series_geo_accession"
//std::string series_geo_accession;


static char **GeoSoftKeys = {
  CSTRING_PLATFORM,
  CSTRING_Platform_title,
  CSTRING_Platform_distribution,
  CSTRING_Platform_technology,
  CSTRING_Platform_organism,
  CSTRING_Platform_manufacturer,
  CSTRING_Platform_manufacture_protocol,
  CSTRING_Platform_catalog_number,
  CSTRING_Platform_web_link,
  CSTRING_Platform_support,
  CSTRING_Platform_coating,
  CSTRING_Platform_description,
  CSTRING_Platform_contributor,
  CSTRING_Platform_pubmed_id,
  CSTRING_Platform_geo_accession,
  CSTRING_Platform_table_begin,
  CSTRING_Platform_table_end,
  CSTRING_SAMPLE,
  CSTRING_Sample_title,
  CSTRING_Sample_supplementary_file,
  CSTRING_Sample_table,
  CSTRING_Sample_source_name_ch,
  CSTRING_Sample_organism_ch,
  CSTRING_Sample_characteristics_ch,
  CSTRING_Sample_biomaterial_provider_ch,
  CSTRING_Sample_treatment_protocol_ch,
  CSTRING_Sample_growth_protocol_ch,
  CSTRING_Sample_molecule_ch,
  CSTRING_Sample_extract_protocol_ch,
  CSTRING_Sample_label_ch,
  CSTRING_Sample_label_protocol_ch,
  CSTRING_Sample_hyb_protocol,
  CSTRING_Sample_scan_protocol,
  CSTRING_Sample_data_processing,
  CSTRING_Sample_description,
  CSTRING_Sample_platform_id,
  CSTRING_Sample_geo_accession,
  CSTRING_Sample_anchor,
  CSTRING_Sample_type,
  CSTRING_Sample_tag_count,
  CSTRING_Sample_tag_length,
  CSTRING_Sample_table_begin,
  CSTRING_Sample_table_end,
  CSTRING_SERIES,
  CSTRING_Series_title,
  CSTRING_Series_summary,
  CSTRING_Series_overall_design,
  CSTRING_Series_pubmed_id,
  CSTRING_Series_web_link,
  CSTRING_Series_contributor,
  CSTRING_Series_variable_,
  CSTRING_Series_variable_description_,
  CSTRING_Series_variable_sample_list_,
  CSTRING_Series_repeats_,
  CSTRING_Series_repeats_sample_list_,
  CSTRING_Series_sample_id,
  CSTRING_Series_geo_accession,
  NULL
};


struct GeoSoftChannel{
  //Number of allowed labels: 1 per channel
  std::string sample_source_name;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::string> sample_organism;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::pair<std::string, std::string> > sample_characteristics;

  //Number of allowed labels: 0 or more per channel
  std::vector<std::string> sample_biomaterial_provider;

  //Number of allowed labels: 0 or more per channel
  std::vector<std::string> sample_treatment_protocol;

  //Number of allowed labels: 0 or more per channel
  std::vector<std::string> sample_growth_protocol;

  //Number of allowed labels: 1 per channel
  std::string sample_molecule;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::string> sample_extract_protocol;

  //Number of allowed labels: 1 per channel
  std::string sample_label;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::string> sample_label_protocol;
};


struct GeoSoft{
  std::string platform;
  std::string platform_title;
  enum platform_distribution_enum platform_distribution;
  enum platform_technology_enum platform_technology;
  std::vector<std::string> platform_organism;
  std::string platform_manufacturer;
  std::vector<std::string> platform_manufacture_protocol;
  std::vector<std::string> platform_catalog_number;
  std::vector<std::string> platform_web_link;
  std::string platform_support;
  std::string platform_coating;
  std::string platform_description;
  std::vector<std::string> platform_contributor;
  std::vector<int> platform_pubmed_id;
  std::string platform_geo_accession;
  std::vector<std::string> platform_table_column_titles;
  std::vector<std::vector<std::string> > platform_table_matrix;
  std::string sample;
  std::string sample_title;
  std::vector<std::string> sample_supplementary_file;
  std::string sample_table;

  std::vector<struct GeoSoftChannel> channel;

  std::vector<std::string> sample_hyb_protocol;
  std::vector<std::string> sample_scan_protocol;
  std::vector<std::string> sample_data_processing;
  std::vector<std::string> sample_description;
  std::string sample_platform_id;
  std::string sample_geo_accession;
  std::string sample_anchor;
  std::string sample_type;
  int sample_tag_count;
  int sample_tag_length;
  std::vector<std::string> sample_table_columns;
  std::vector<std::vector<std::string> > sample_table_matrix;
  std::string series;
  std::string series_title;
  std::vector<std::string> series_summary;
  std::string series_overall_design;
  std::vector<std::string> series_pubmed_id;
  std::vector<std::string> series_web_link;
  std::vector<std::string> series_contributor;
  std::vector<std::string> series_variable;
  std::vector<std::string> series_variable_description;
  std::vector<std::string> series_variable_sample_list;
  std::vector<std::string> series_repeats;
  std::vector<std::string> series_repeats_sample_list;
  std::vector<std::string> series_sample_id;
  std::string series_geo_accession;
};

int loadGeoSoftFile(const char *fp, struct GeoSoft **contents);

#endif
